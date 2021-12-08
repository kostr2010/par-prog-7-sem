#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mpi.h>

#define ISIZE 10000
#define JSIZE 10000

#define PROC_MASTER 0
#define OVERLAP_J 3
#define OVERLAP_I 1

#define BENCHMARK
// #undef BENCHMARK

#define DEBUG
#undef DEBUG

struct pair_t {
  int begin;
  int end;
};
typedef struct pair_t pair_t;

int main(int argc, char **argv) {
  int i = 0, j = 0;

  int err = 0;
  err = MPI_Init(&argc, &argv);
  assert(err == 0);

  // variables that will be used by every parallel block
  int comm_sz = 0;
  int comm_rank = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

  err = MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  assert(err == 0);
  err = MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
  assert(err == 0);

  // allocate memory to store results
  // since operations inside loops aren'i complicated, there is no reason to
  // make dynamic load distribution (it is also much harder to do so). same
  // reasoning applies to all loops in this lab
  // so we'll need separate buffer for each process to store data to
  int subproc_n_rows = ISIZE / comm_sz;
  int subproc_n_elems = subproc_n_rows * JSIZE;

  double *subproc_buf = (double *)calloc(subproc_n_elems, sizeof(double));
  double *final_buf = (comm_rank == PROC_MASTER)
                          ? ((double *)calloc(ISIZE * JSIZE, sizeof(double)))
                          : NULL;

  // syncronize after allocation
  MPI_Barrier(MPI_COMM_WORLD);

  // first loop
  {
    for (i = 0; i < subproc_n_rows; i++) {
      for (j = 0; j < JSIZE; j++) {
        subproc_buf[ISIZE * i + j] = 10 * (subproc_n_rows * comm_rank + i) + j;
      }
    }

    // since there are no more than n * JSIZE operations in this loop, and only
    // one thread is busy calculating it, it's negligible when calculating
    // execution time in comparison to previous nested loop, where there  are
    // ISIZE * JSIZE / n operations
    if (comm_rank == PROC_MASTER) {
      for (i = subproc_n_rows * comm_sz; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
          final_buf[i * ISIZE + j] = 10 * i + j;
        }
      }
    }
  }

  // gather results
  MPI_Gather(subproc_buf, subproc_n_elems, MPI_DOUBLE, final_buf,
             subproc_n_elems, MPI_DOUBLE, PROC_MASTER, MPI_COMM_WORLD);

  // indexes of first and last i points for each process
  pair_t *map = (pair_t *)calloc(comm_sz, sizeof(pair_t));

  subproc_n_elems = (JSIZE - OVERLAP_J) / comm_sz;

  if (comm_sz > JSIZE) {
    for (i = 0; i < comm_sz; ++i) {
      if (i < JSIZE - OVERLAP_J) {
        map[i].begin = i + OVERLAP_J;
        map[i].end = map[i].begin + 1;
      } else {
        map[i].begin = -1;
        map[i].end = -1;
      }
    }
  } else {
    for (i = 0; i < comm_sz; ++i) {
      map[i].begin = OVERLAP_J + subproc_n_elems * i;

      if (i == comm_sz - 1) {
        map[i].end = JSIZE - 1;
      } else {
        map[i].end = map[i].begin + subproc_n_elems;
      }
    }
  }

#ifdef DEBUG
  if (comm_rank == PROC_MASTER) {
    for (i = 0; i < comm_sz; i++) {
      printf("proc %2d shall calculate nodes [%2d, %2d) of x (%d in total)\n",
             i, map[i].begin, map[i].end, map[i].end - map[i].begin);
    }
  }
#endif

  // each process stores only his span, and OVERLAP_J nodes extra - OVERLAP_J to
  // the left
  // DONE: it's enough to allocate only 2 * (map[comm_rank].end -
  // map[comm_rank].begin + OVERLAP_J) elements and iterate using i % 2
  subproc_buf = (double *)realloc(
      subproc_buf, ISIZE * (subproc_n_elems + OVERLAP_J) * sizeof(double));
  explicit_bzero(subproc_buf,
                 ISIZE * (subproc_n_elems + OVERLAP_J) * sizeof(double));

  assert(subproc_buf != NULL);

  // syncronize after allocation
  MPI_Barrier(MPI_COMM_WORLD);

  // second loop (conditioned entry)
  if (map[comm_rank].begin != -1 && map[comm_rank].end != -1) {
    for (i = 1; i < ISIZE; ++i) {
      int subproc_n_elems = map[comm_rank].end - map[comm_rank].begin;

#ifdef DEBUG
      if (comm_rank == PROC_MASTER) {
        printf("// ROOT %d\n", i);
        for (int k = 0; k < ISIZE; ++k) {
          for (int l = 0; l < JSIZE; ++l) {
            printf("%f ", final_buf[k * ISIZE + l]);
          }
          printf("\n");
        }
        printf("// ROOT END %d\n", i);
      } else {
        printf("// PROC %d %d\n", comm_rank, i);
        for (int p = 0; p < 2; ++p) {
          for (int l = 0; l < subproc_n_elems + OVERLAP_J; ++l) {
            printf("%f ", subproc_buf[p * (subproc_n_elems + OVERLAP_J) + l]);
          }
          printf("\n");
        }
        printf("// PROC %d END %d\n", comm_rank, i);
      }
#endif

      MPI_Status status;

      if (comm_rank == PROC_MASTER) {
        // send i - 1 step
        int offset = 0;

        for (int k = 0; k < comm_sz; ++k) {
          int subproc_k_n_elems = map[k].end - map[k].begin;

          if (k == PROC_MASTER) {
            offset += subproc_k_n_elems;
            continue;
          }

          MPI_Send(&final_buf[JSIZE * (i - OVERLAP_I) + offset],
                   subproc_k_n_elems, MPI_DOUBLE, k, 0, MPI_COMM_WORLD);

#ifdef DEBUG
          printf("ROOT process %d sent %d elements [%d %d) to process %d as "
                 "i-1 th\n",
                 comm_rank, subproc_k_n_elems, JSIZE * (i - 1) + offset,
                 JSIZE * (i - OVERLAP_I) + offset + subproc_k_n_elems, k);
#endif

          offset += subproc_k_n_elems;
        }
      } else {
        // recieve i - 1 step
        MPI_Recv(
            &subproc_buf[(subproc_n_elems + OVERLAP_J) * ((i - OVERLAP_I) % 2)],
            subproc_n_elems, MPI_DOUBLE, PROC_MASTER, 0, MPI_COMM_WORLD,
            &status);

#ifdef DEBUG
        printf("process %d recieved %d elements [%d %d) from ROOT as i-1 th\n",
               comm_rank, subproc_n_elems,
               (subproc_n_elems + OVERLAP_J) * ((i - OVERLAP_I) % 2),
               (subproc_n_elems + OVERLAP_J) * ((i - OVERLAP_I) % 2) +
                   subproc_n_elems);
#endif
      }

      MPI_Barrier(MPI_COMM_WORLD);

      for (j = 0; j < subproc_n_elems; ++j) {
        if (comm_rank == PROC_MASTER) {
          final_buf[JSIZE * i + (map[comm_rank].begin + j)] =
              sin(0.00001 * final_buf[JSIZE * (i - OVERLAP_I) +
                                      (j + map[comm_rank].begin - OVERLAP_J)]);
        } else {
          subproc_buf[(subproc_n_elems + OVERLAP_J) * (i % 2) +
                      (j + OVERLAP_J)] =
              sin(0.00001 * subproc_buf[(subproc_n_elems + OVERLAP_J) *
                                            ((i - OVERLAP_I) % 2) +
                                        j]);
        }
      }

      // recieve directly calculated values from each process
      if (comm_rank == PROC_MASTER) {
        int offset = 0;

        for (int k = 0; k < comm_sz; ++k) {
          int subproc_k_n_elems = map[k].end - map[k].begin;

          if (k == PROC_MASTER) {
            offset += subproc_k_n_elems;
            continue;
          }

          MPI_Recv(&final_buf[JSIZE * i + OVERLAP_J + offset],
                   subproc_k_n_elems, MPI_DOUBLE, k, 0, MPI_COMM_WORLD,
                   &status);

#ifdef DEBUG
          printf("ROOT process recieved %d elements [%d %d) from process %d\n",
                 subproc_k_n_elems, JSIZE * i + OVERLAP_J + offset,
                 JSIZE * i + OVERLAP_J + offset + subproc_k_n_elems, k);
#endif

          offset += subproc_k_n_elems;
        }
      } else {
        MPI_Send(
            &subproc_buf[(subproc_n_elems + OVERLAP_J) * (i % 2) + OVERLAP_J],
            subproc_n_elems, MPI_DOUBLE, PROC_MASTER, 0, MPI_COMM_WORLD);
#ifdef DEBUG
        printf("process %d sent %d elements [%d %d) to ROOT\n", comm_rank,
               subproc_n_elems,
               (subproc_n_elems + OVERLAP_J) * (i % 2) + OVERLAP_J,
               (subproc_n_elems + OVERLAP_J) * (i % 2) + OVERLAP_J +
                   subproc_n_elems);
#endif
      }

      MPI_Barrier(MPI_COMM_WORLD);
    }
  }

#ifndef BENCHMARK
  if (comm_rank == PROC_MASTER) {
    FILE *ff = fopen("result-par.txt", "w");

    for (i = 0; i < ISIZE; i++) {
      for (j = 0; j < JSIZE; j++) {
        fprintf(ff, "%f ", final_buf[i * ISIZE + j]);
      }
      fprintf(ff, "\n");
    }

    fclose(ff);

    free(final_buf);
  }

  free(subproc_buf);

#endif

  MPI_Finalize();

  return 0;
}
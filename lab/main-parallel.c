#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define ISIZE 10000
#define JSIZE 10000

#define BENCHMARK
// #undef BENCHMARK

int main(int argc, char **argv) {
  int i = 0, j = 0;

  MPI_Init(&argc, &argv);

  // variables that will be used by every parallel block
  int comm_sz = 0;
  int comm_rank = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

  // since operations inside loops aren't complicated, there is no reason to
  // make dynamic load distribution (it is also much harder to do so). same
  // reasoning applies to all loops in this lab
  // so we'll need separate buffer for each process to store data to
  const int subproc_n_rows = ISIZE / comm_sz;
  const int subproc_n_elems = subproc_n_rows * JSIZE;

  double *subproc_buf = (double *)calloc(subproc_n_elems, sizeof(double));
  double *final_buf = (comm_rank == comm_sz - 1)
                          ? ((double *)calloc(ISIZE * JSIZE, sizeof(double)))
                          : NULL;

  // syncronize after allocation
  MPI_Barrier(MPI_COMM_WORLD);

  // i will single out every nested loop in it's own context, thus it will be
  // easier to separate information regagding each loop from each other

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
    if (comm_rank == comm_sz - 1) {
      for (i = subproc_n_rows * comm_sz; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
          final_buf[i * ISIZE + j] = 10 * i + j;
        }
      }
    }
  }

  // second loop
  {
    for (i = 0; i < subproc_n_rows; i++) {
      for (j = 0; j < JSIZE; j++) {
        subproc_buf[ISIZE * i + j] = sin(0.00001 * subproc_buf[ISIZE * i + j]);
      }
    }

    // since there are no more than n * JSIZE operations in this loop, and only
    // one thread is busy calculating it, it's negligible when calculating
    // execution time in comparison to previous nested loop, where there  are
    // ISIZE * JSIZE / n operations
    if (comm_rank == comm_sz - 1) {
      for (i = subproc_n_rows * comm_sz; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
          final_buf[i * ISIZE + j] = sin(0.00001 * final_buf[i * ISIZE + j]);
        }
      }
    }
  }

  // gather results
  MPI_Gather(subproc_buf, subproc_n_elems, MPI_DOUBLE, final_buf,
             subproc_n_elems, MPI_DOUBLE, comm_sz - 1, MPI_COMM_WORLD);

  if (comm_rank == comm_sz - 1) {
#ifndef BENCHMARK
    FILE *ff = fopen("result-par.txt", "w");

    for (i = 0; i < ISIZE; i++) {
      for (j = 0; j < JSIZE; j++) {
        fprintf(ff, "%f ", final_buf[i * ISIZE + j]);
      }
      fprintf(ff, "\n");
    }

    fclose(ff);
#endif

    free(final_buf);
  }

  free(subproc_buf);

  MPI_Finalize();

  return 0;
}
// task 4. multiply matrices L * R = F
// done without extra task (*)

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// generate numbers in range [-RAND_MAGNITUDE; RAND_MAGNITUDE]
#define RAND_MAGNITUDE 100

struct Matrix {
  double *data;
  unsigned long n_rows;
  unsigned long n_cols;
};

int Init(struct Matrix *m, unsigned long rows, unsigned long cols, char type);
void Destroy(struct Matrix *m);
void Dump(struct Matrix *m, FILE *f);

int main(int argc, char *argv[]) {
  assert(argc == 5);

  printf("%d threads run\n\n", omp_get_max_threads());

  // no safety measures were taken regarding overflowing, so be cautious with
  // big numbers
  unsigned long n_rows_l = strtoul(argv[1], NULL, 10);
  unsigned long n_cols_l = strtoul(argv[2], NULL, 10);
  unsigned long n_rows_r = strtoul(argv[3], NULL, 10);
  unsigned long n_cols_r = strtoul(argv[4], NULL, 10);

  assert(n_cols_l == n_rows_r);

  unsigned long n_rows_f = n_rows_l;
  unsigned long n_cols_f = n_cols_r;

  struct Matrix l = {NULL, 0, 0};
  struct Matrix r = {NULL, 0, 0};
  struct Matrix f = {NULL, 0, 0};

  srand(time(NULL));
  assert(Init(&l, n_rows_l, n_cols_l, 0) == 0);
  assert(Init(&r, n_rows_r, n_cols_r, 0) == 0);
  assert(Init(&f, n_rows_f, n_cols_f, 'Z') == 0);

  unsigned long i = 0, j = 0, k = 0;
#pragma omp parallel for private(i, j, k) shared(l, r, f) schedule(dynamic)    \
    collapse(2)
  for (i = 0; i < l.n_rows; i++) {
    for (j = 0; j < r.n_cols; j++) {
      for (k = 0; k < l.n_cols; k++) {
        f.data[i * f.n_cols + j] +=
            l.data[i * l.n_cols + k] * r.data[k * r.n_cols + j];
      }
    }
  }

  FILE *out = fopen("res.txt", "w");

  Dump(&l, out);
  fprintf(out, "*\n");
  Dump(&r, out);
  fprintf(out, "=\n");
  Dump(&f, out);

  fclose(out);

  Destroy(&l);
  Destroy(&r);
  Destroy(&f);

  return 0;
}

int Init(struct Matrix *m, unsigned long n_rows, unsigned long n_cols,
         char type) {
  if (m == NULL) {
    return -1;
  }

  m->data = (double *)calloc(n_rows * n_cols, sizeof(double));

  if (m->data == NULL) {
    return -2;
  }

  m->n_cols = n_cols;
  m->n_rows = n_rows;

  switch (type) {
  case 'Z':
    return 0;
  case 'I':
  case 'E':
    if (n_rows != n_cols) {
      return -3;
    }

    for (unsigned long i = 0; i < n_rows; i++) {
      m->data[i * n_rows + i] = 1.0;
    }
    return 0;
  default:
    for (unsigned long i = 0; i < n_rows; i++) {
      for (unsigned long j = 0; j < n_cols; j++) {
        m->data[i * n_cols + j] =
            (((double)rand() - 0.5 * (double)RAND_MAX) / (double)RAND_MAX) *
            RAND_MAGNITUDE;
      }
    }
  }

  return 0;
}

void Destroy(struct Matrix *m) {
  if (m == NULL || m->data == NULL) {
    return;
  }

  free(m->data);
}

void Dump(struct Matrix *m, FILE *f) {
  assert(m != NULL);
  assert(m->data != NULL);

  fprintf(f, "{");

  for (unsigned long i = 0; i < m->n_rows; i++) {
    fprintf(f, "{");

    for (unsigned long j = 0; j < m->n_cols; j++) {
      fprintf(f, "%3.6f", m->data[i * m->n_cols + j]);

      if (j != m->n_cols - 1) {
        fprintf(f, ", ");
      }
    }

    fprintf(f, "}");
    if (i != m->n_rows - 1) {
      fprintf(f, ", ");
    }
  }

  fprintf(f, "}\n");
}

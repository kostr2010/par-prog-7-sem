#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ISIZE 10000
#define JSIZE 10000

#define BENCHMARK
// #undef BENCHMARK

int main() {
  double **a = (double **)calloc(ISIZE, sizeof(double *));
  int i = 0, j = 0;

  for (i = 0; i < ISIZE; ++i) {
    a[i] = (double *)calloc(JSIZE, sizeof(double));
  }

  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = 10 * i + j;
    }
  }

  for (i = 1; i < ISIZE; i++) {
    for (j = 3; j < JSIZE - 1; j++) {
      a[i][j] = sin(0.00001 * a[i - 1][j - 3]);
    }
  }

#ifndef BENCHMARK
  FILE *ff = fopen("result.txt", "w");

  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      fprintf(ff, "%f ", a[i][j]);
    }
    fprintf(ff, "\n");
  }

  fclose(ff);
#endif

  return 0;
}
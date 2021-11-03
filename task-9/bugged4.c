/******************************************************************************
 * ЗАДАНИЕ: bugged4.c
 * ОПИСАНИЕ:
 *   Очень простая программа с segmentation fault.
 ******************************************************************************/

/*
 * изначальные ошибки компиляции:
 * - unused argc, argv (но это из-за параметра сборки -Werror)
 *
 * изначальные ошибки кода:
 * - segfault
 *
 * решения:
 * - уберем неиспользуемые аргументы
 * - из gdb узнаем, что падение происходит до входа в параллельную секцию, т.е.
 *   при создании массива. действительно, он слишком большой для стековой
 *   аллокации, поэтому его лучше аллоцировать на куче. т.к. a - приватная, то в
 *   параллельном регионе ее нужно инициализировать отдельно для каждого потока
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1024

int main() {
  int nthreads, tid, i, j;
  double **a;

#pragma omp parallel shared(nthreads) private(i, j, tid, a)
  {
    a = (double **)calloc(N, sizeof(double *));
    for (int k = 0; k < N; ++k) {
      a[k] = (double *)calloc(N, sizeof(double));
    }

    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    }
    printf("Thread %d starting...\n", tid);

    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        a[i][j] = tid + i + j;

    printf("Thread %d done. Last element= %f\n", tid, a[N - 1][N - 1]);
  }
}

/******************************************************************************
 * ЗАДАНИЕ: bugged3.c
 * ОПИСАНИЕ:
 *   Ошибка времени выполнения.
 ******************************************************************************/

/*
 * изначальные ошибки компиляции:
 * - unused argc, argv (но это из-за параметра сборки -Werror)
 *
 * изначальные ошибки кода:
 * - бесконечное время выполнения
 *
 * решения:
 * - уберем неиспользуемые аргументы
 * - уберем барьер на 89 строке, т.к. его достигают не все потоки, из-за чего
 *   программа остается в дедлоке (из вывода в консоль можно видеть, что треды,
 *   исполняющие секции не доходят до следующего барьера, который на 66 строке)
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 50

int main() {
  int i, nthreads, tid, section;
  float a[N], b[N], c[N];
  void print_results(float array[N], int tid, int section);

  for (i = 0; i < N; i++)
    a[i] = b[i] = i * 1.0;

#pragma omp parallel private(c, i, tid, section)
  {
    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    }

#pragma omp barrier
    printf("Thread %d starting...\n", tid);
#pragma omp barrier

#pragma omp sections nowait
    {
#pragma omp section
      {
        section = 1;
        for (i = 0; i < N; i++)
          c[i] = a[i] * b[i];
        print_results(c, tid, section);
      }

#pragma omp section
      {
        section = 2;
        for (i = 0; i < N; i++)
          c[i] = a[i] + b[i];
        print_results(c, tid, section);
      }
    }

#pragma omp barrier
    printf("Thread %d exiting...\n", tid);
  }
}

void print_results(float array[N], int tid, int section) {
  int i, j;

  j = 1;
#pragma omp critical
  {
    printf("\nThread %d did section %d. The results are:\n", tid, section);
    for (i = 0; i < N; i++) {
      printf("%e  ", array[i]);
      j++;
      if (j == 6) {
        printf("\n");
        j = 1;
      }
    }
    printf("\n");
  }

  // #pragma omp barrier
  printf("Thread %d done and synchronized.\n", tid);
}

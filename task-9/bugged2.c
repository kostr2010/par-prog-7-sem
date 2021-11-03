/******************************************************************************
 * ЗАДАНИЕ: bugged2.c
 * ОПИСАНИЕ:
 *   Еще одна программа на OpenMP с багом.
 ******************************************************************************/

/*
 * изначальные ошибки компиляции:
 * - unused argc, argv (но это из-за параметра сборки -Werror)
 *
 * изначальные ошибки кода:
 * - tid - не приватная переменная, соответственно значение в этой переменной
 *   будет некорректное в общем случае
 * - аналогично для i
 *
 * решения:
 * - уберем неиспользуемые аргументы
 * - сделаем tid, i приватными в параллельном регионе
 * - добавим reduction(+ : total) для корректной обработки цикла
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int nthreads, i, tid;
  float total;

#pragma omp parallel private(tid, i)
  {
    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    }
    printf("Thread %d is starting...\n", tid);

#pragma omp barrier

    total = 0.0;
#pragma omp for schedule(dynamic, 10) reduction(+ : total)
    for (i = 0; i < 10; i++)
      total = total + i * 1.0;

    printf("Thread %d is done! Total= %f\n", tid, total);
  }
}

/******************************************************************************
 * ЗАДАНИЕ: bugged6.c
 * ОПИСАНИЕ:
 *   Множественные ошибки компиляции
 ******************************************************************************/

/*
 * изначальные ошибки компиляции:
 * - unused argc, argv (но это из-за параметра сборки -Werror)
 * - bugged6.c:37:9: error: reduction variable ‘sum’ is private in outer context
 * - bugged6.c:42:1: error: control reaches end of non-void function
 *   [-Werror=return-type]
 *
 * изначальные ошибки кода:
 * -
 *
 * решения:
 * - уберем неиспользуемые аргументы
 * - т.к. неясно как функция должна была работать (у нее нет ни аргументов ни
 *   возвращаемого значения, ее можно раскрыть в main (заинлайнить)). для этого
 *   скопируем тело функции в параллельный регион, удалим неиспользуемые
 *   переменные, сделаем общий счетчик i private для всех потоков
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100

float a[VECLEN], b[VECLEN];

// float dotprod() {
//   int i, tid;
//   float sum;

//   tid = omp_get_thread_num();
// #pragma omp for reduction(+ : sum) shared(sum)
//   for (i = 0; i < VECLEN; i++) {
//     sum = sum + (a[i] * b[i]);
//     printf("  tid= %d i=%d\n", tid, i);
//   }
// }

int main() {
  int i;
  float sum;

  for (i = 0; i < VECLEN; i++)
    a[i] = b[i] = 1.0 * i;
  sum = 0.0;

#pragma omp parallel private(i) shared(sum)
  {
    // dotprod();
#pragma omp for reduction(+ : sum)
    for (i = 0; i < VECLEN; i++) {
      sum = sum + (a[i] * b[i]);
      printf("  tid= %d i=%d\n", omp_get_thread_num(), i);
    }
  }

  printf("Sum = %f\n", sum);

  return 0;
}

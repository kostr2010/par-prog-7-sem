/******************************************************************************
 * ЗАДАНИЕ: bugged1.c
 * ОПИСАНИЕ:
 *   Данная программа демонстрирует использование конструкции 'parallel for'.
 *   Однако, данный код вызывает ошибки компиляции.
 ******************************************************************************/

/*
 * изначальные ошибки компиляции:
 * - bugged1.c:28:5: error: for statement expected before ‘{’ token
 * - unused argc, argv (но это из-за параметра сборки -Werror)
 *
 * изначальные ошибки кода:
 *
 * решения:
 * - перед '{' ожидается statement, в нашем случае for, добавим его
 * - уберем неиспользуемые аргументы
 * - перенесем вызов omp_get_thread_num() в тело printf(...), удалим
 *   неиспользуемую переменную
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 50
#define CHUNKSIZE 5

int main() {
  int i, chunk;
  float a[N], b[N], c[N];

  for (i = 0; i < N; i++)
    a[i] = b[i] = i * 1.0;
  chunk = CHUNKSIZE;

#pragma omp parallel for shared(a, b, c, chunk) private(i)                     \
    schedule(static, chunk)
  for (i = 0; i < N; i++) {
    c[i] = a[i] + b[i];
    printf("tid= %d i= %d c[i]= %f\n", omp_get_thread_num(), i, c[i]);
  }
}

// task 2. sum up to M
// done extra tasks (*)

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("%d threads run\n\n", omp_get_max_threads());

  assert(argc == 2);

  long num = atol(argv[1]);
  long sum = 0;

#pragma omp parallel shared(sum)
  {
#pragma omp for reduction(+ : sum) schedule(guided)
    for (long i = 0; i < num; ++i) {
      sum += i;
    }
  }

  printf("sum = %ld\n", sum);

  return 0;
}
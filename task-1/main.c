// task 1. hello world
// done extra task (*)

#include <omp.h>
#include <stdio.h>

int main() {
  printf("%d threads run\n\n", omp_get_max_threads());

  int last = omp_get_max_threads() - 1;

  printf("hello world\n");

#pragma omp parallel shared(last)
  {
    int my_id = omp_get_thread_num();

    while (last > my_id) {
    }

    printf("%d\n", my_id);
    --last;
  }
  return 0;
}
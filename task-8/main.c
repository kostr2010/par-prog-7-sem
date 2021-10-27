// task n
// done without extra task (*)

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(/* int argc, char *argv[] */) {
  // assert(argc == );

  printf("%d threads run\n\n", omp_get_max_threads());

  return 0;
}
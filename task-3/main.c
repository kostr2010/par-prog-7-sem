// task 3. parallel array modification
// done without extra task (*)

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define ARR_SZ 100000

double a[ARR_SZ] = {0.0};
double b[ARR_SZ] = {0.0};

void Init();

int main() {
  printf("%d threads run\n\n", omp_get_max_threads());

  Init();

  int i = 0;

// dynamic because multiplying big numbers is slower
#pragma omp parallel for private(i) schedule(dynamic)
  for (i = 1; i < ARR_SZ - 1; i++) {
    b[i] = a[i - 1] * a[i] * a[i + 1] / 3.0;
  }

  b[0] = a[0];
  b[ARR_SZ - 1] = a[ARR_SZ - 1];

  FILE *f = fopen("res.txt", "w");

  for (i = 0; i < ARR_SZ; ++i) {
    fprintf(f, "%.0f\n", b[i]);
  }

  fclose(f);

  return 0;
}

void Init() {
  int i = 0;
#pragma omp parallel for private(i) schedule(static)
  for (i = 0; i < ARR_SZ; i++) {
    a[i] = (double)i + 1; // or b will be array of 0's
  }
}
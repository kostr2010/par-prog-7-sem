// task 5. calculate exponent
// done with extra task (*)

#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

enum Series {
  EXP,
  SIN,
  COS,
};

long double InverseFact(unsigned long n);

int main(int argc, char *argv[]) {
  assert(argc == 4);

  printf("%d threads run\n\n", omp_get_max_threads());

  int n_iter = atoi(argv[1]);
  int type = atoi(argv[2]);
  double x = strtod(argv[3], NULL);

  assert(n_iter > 0);

  switch (type) {
  // sin(x)
  case SIN: {
    double sum = 0;

    unsigned long i = 0;
#pragma omp parallel for private(i) reduction(+ : sum) schedule(dynamic)
    for (i = 0; i < n_iter; i++) {
      sum += InverseFact(2.0 * i + 1.0) * pow(x, 2.0 * (double)i + 1.0) *
             pow(-1.0, (double)i);
    }

    printf("series sum = %.30f\n", sum);
    printf("sin(x)     = %.30f\n", sin(x));
    printf("diff       = %.30F\n", fabs(sum - sin(x)));

    return 0;
  } break;
  // cos(x)
  case COS: {
    double sum = 1;

    unsigned long i = 0;
#pragma omp parallel for private(i) reduction(+ : sum) schedule(dynamic)
    for (i = 1; i < n_iter; i++) {
      sum +=
          InverseFact(2.0 * i) * pow(x, 2.0 * (double)i) * pow(-1.0, (double)i);
    }

    printf("series sum = %.30f\n", sum);
    printf("cos(x)     = %.30f\n", cos(x));
    printf("diff       = %.30F\n", fabs(sum - cos(x)));

    return 0;
  } break;
  // exp(x)
  case EXP:
  default: {
    double sum = 0;

    unsigned long i = 0;
#pragma omp parallel for private(i) reduction(+ : sum) schedule(dynamic)
    for (i = 0; i < n_iter; i++) {
      sum += InverseFact(i) * pow(x, (double)i);
    }

    printf("series sum = %.30f\n", sum);
    printf("exp(x)     = %.30f\n", exp(x));
    printf("diff       = %.30F\n", fabs(sum - exp(x)));

    return 0;
  } break;
  }

  return 0;
}

long double InverseFact(unsigned long n) {
  if (n == 0) {
    return 1.0;
  }

  long double fact = 1;

  for (unsigned long i = 1; i <= n; ++i) {
    fact /= i;
  }

  return fact;
}
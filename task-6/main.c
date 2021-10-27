// task 6. prime numbers search
// done without extra task (*)

#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNSET 0x00
#define SET 0xFF

enum AlgType {
  ERATOSTHENES,
  ATKIN,
};

int main(int argc, char *argv[]) {
  assert(argc == 3);

  printf("%d threads run\n\n", omp_get_max_threads());

  int num = atoi(argv[1]);
  int alg = atoi(argv[2]);

  if (num <= 5) {
    printf("primes up to 5 are: 2 3 5\n");
    return 0;
  }

  uint8_t *sieve = (uint8_t *)calloc(1 + num, sizeof(uint8_t));

  int num_sqrt = (int)sqrt(num) + 1;

  int i = 0;
  int j = 0;

  switch (alg) {
  // atkin
  case ATKIN: {
    // setup
    memset(sieve, SET, (1 + num) * sizeof(uint8_t));
    sieve[2] = UNSET;
    sieve[3] = UNSET;

#pragma omp parallel for private(i, j) shared(sieve) schedule(static)          \
    collapse(2)
    for (i = 1; i < num_sqrt; i++) {
      for (j = 1; j < num_sqrt; j++) {
        int n = (4 * i * i) + (j * j);
        if (n <= num && (n % 12 == 1 || n % 12 == 5))
          sieve[n] ^= SET;

        n = (3 * i * i) + (j * j);
        if (n <= num && n % 12 == 7)
          sieve[n] ^= SET;

        n = (3 * i * i) - (j * j);
        if (i > j && n <= num && n % 12 == 11)
          sieve[n] ^= SET;
      }
    }

    // mark squares, since algoithm ignores them
#pragma omp parallel for private(i, j) shared(sieve) schedule(static)
    for (i = 5; i < num_sqrt; i++) {
      if (sieve[i] == UNSET) {
        for (j = i * i; j < num; j += i * i)
#pragma omp critical
          sieve[j] = SET;
      }
    }
  } break;
  // eratosthenes
  case ERATOSTHENES:
  default: {
#pragma omp parallel for private(i) shared(sieve) schedule(dynamic)
    for (i = 2; i < num_sqrt; i++) {
      if (sieve[i] == SET) {
        continue;
      }

      for (int j = 2 * i; j <= num; j += i) {
#pragma omp critical
        sieve[j] = SET;
      }
    }
  } break;
  }

  int count = 0;
#pragma omp parallel for private(i) shared(sieve) reduction(+: count) schedule(static)
  for (i = 2; i <= num; i++) {
    if (sieve[i] == UNSET) {
      count++;
    }
  }

  printf("there are %d primes before %d:\n", count, num);
  for (i = 2; i <= num; i++) {
    if (sieve[i] == UNSET) {
      printf("%d ", i);
    }
  }

  printf("\n");

  return 0;
}
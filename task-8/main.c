// task 8, quick sort
// done with extra task (*)

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int32_t *Init(int *sz, const int argc, char *argv[]);

void Quicksort(int32_t *array, size_t beg, size_t end);
size_t MovePivot(int32_t *array, size_t beg, size_t end);

int main(int argc, char *argv[]) {
  printf("%d threads run\n\n", omp_get_max_threads());

  int arr_sz = 0;
  int32_t *arr = Init(&arr_sz, argc, argv);

  if (arr_sz <= 0) {
    return -10;
  }

#pragma omp parallel
  {
#pragma omp single
    Quicksort(arr, 0, arr_sz - 1);
  }

  FILE *f = fopen("out.txt", "w");

  if (f == NULL) {
    printf("can't open output file\n");

    return -20;
  }

  fprintf(f, "%d\n", arr_sz);
  for (int i = 0; i < arr_sz; i++) {
    fprintf(f, "%d\n", arr[i]);
  }

  fclose(f);

  return 0;
}

int32_t *Init(int *sz, const int argc, char *argv[]) {
  assert(argc >= 2);
  assert(sz != NULL);

  int input_t = atoi(argv[1]);

  int n_elements = 0;
  int32_t *array = NULL;

  switch (input_t) {
  case 0: {
    assert(argc == 3);

    FILE *f = fopen(argv[2], "r");

    if (f == NULL) {
      printf("can't open file <%s>\n", argv[2]);

      return NULL;
    }

    if (fscanf(f, "%d", &n_elements) != 1) {
      printf("error. unexpected format\n");

      return NULL;
    }

    array = (int32_t *)calloc(n_elements, sizeof(int32_t));
    assert(array != NULL);

    int n = n_elements;
    while (!feof(f) && n--) {
      if (fscanf(f, "%d", array + (n_elements - n - 1)) != 1) {
        printf("error. unexpected data format\n");

        return NULL;
      }
    }

    printf("scanned %d elements from <%s>:\n", n_elements, argv[2]);

    fclose(f);
  } break;
  case 1:
  default: {
    if (scanf("%d", &n_elements) != 1) {
      printf("error. unexpected format\n");

      return NULL;
    }

    array = (int32_t *)calloc(n_elements, sizeof(int32_t));
    assert(array != NULL);

    for (int i = 0; i < n_elements; i++) {
      if (scanf("%d", array + i) != 1) {
        printf("\nerror. unexpected data format\n");

        return NULL;
      }
    }

    printf("scanned %d elements from console:\n", n_elements);
  } break;
  }

  for (int i = 0; i < n_elements; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");

  *sz = n_elements;

  return array;
}

void Quicksort(int32_t *array, size_t beg, size_t end) {
  if (beg >= end) {
    return;
  }

  int pivot = MovePivot(array, beg, end);

#pragma omp task shared(array)
  Quicksort(array, beg, pivot);

#pragma omp task shared(array)
  Quicksort(array, pivot + 1, end);
}

size_t MovePivot(int32_t *array, size_t beg, size_t end) {
  int32_t p = array[(size_t)(0.5 * (beg + end))];

  while (beg <= end) {
    while (array[beg] < p) {
      beg++;
    }

    while (array[end] > p) {
      end--;
    }

    if (beg >= end) {
      break;
    }

    int32_t swap = array[end];
    array[end] = array[beg];
    array[beg] = swap;

    beg++;
    end--;
  }

  return end;
}
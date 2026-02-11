#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// in order to send size with the array we need to encapsulate it
// into a struct and send the struct
struct Array {
  int size;
  int *data;
};

void *maximum(void *param);
void *minimum(void *param);
void *average(void *param);

int main(int argc, char *argv[]) {
  // We need three tid as we need to run 3 threads
  // but we can use single attr as we use the default
  // attributes and pthread_create doesn't modify it.
  pthread_t tid[3];    /* the thread identifier */
  pthread_attr_t attr; /* set of thread attributes */

  // that is how we define struct in C
  struct Array arr;
  
  // define pointers to hold the return values
  int *max, *min, *avg;

  if (argc < 2) {
    fprintf(stderr, "usage: a.out <integer values>\n");
    return -1;
  }

  arr.size = argc - 1;
  arr.data = (int *)malloc(arr.size * sizeof(int));
  for (int i = 0; i < arr.size; i++) {
    arr.data[i] = atoi(argv[i + 1]);
  }

  /* get the default attributes */
  pthread_attr_init(&attr);

  /* create threads */
  pthread_create(&tid[0], &attr, maximum, &arr);
  pthread_create(&tid[1], &attr, minimum, &arr);
  pthread_create(&tid[2], &attr, average, &arr);

  /* wait for the thread to exit */
  pthread_join(tid[0], (void **)&max);
  pthread_join(tid[1], (void **)&min);
  pthread_join(tid[2], (void **)&avg);

  printf("max = %d\n", *max);
  printf("min = %d\n", *min);
  printf("avg = %d\n", *avg);

  free(arr.data);
  // malloc in functions
  free(max);
  free(min);
  free(avg);
}

/* The thread will begin control in this function */
void *maximum(void *param) {
  struct Array *arr = (struct Array *)param;
  int *max = malloc(sizeof(int));

  *max = arr->data[0];
  for (int i = 1; i < arr->size; i++) {
    if (arr->data[i] > *max) {
      *max = arr->data[i];
    }
  }

  return (void *)max;
}

void *minimum(void *param) {
  struct Array *arr = (struct Array *)param;
  int *min = malloc(sizeof(int));

  *min = arr->data[0];
  for (int i = 1; i < arr->size; i++) {
    if (arr->data[i] < *min) {
      *min = arr->data[i];
    }
  }

  return (void *)min;
}

void *average(void *param) {
  struct Array *arr = (struct Array *)param;
  int *avg = malloc(sizeof(int));

  for (int i = 0; i < arr->size; i++) {
    *avg += arr->data[i];
  }

  *avg = *avg / arr->size;

  return (void *)avg;
}

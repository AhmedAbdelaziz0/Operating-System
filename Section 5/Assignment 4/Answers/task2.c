#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int max;                   /* this data is shared by the thread(s) */
void *runner(void *param); /* threads call this function */
int size;

int main(int argc, char *argv[]) {
  pthread_t tid;       /* the thread identifier */
  pthread_attr_t attr; /* set of thread attributes */

  if (argc < 2) {
    fprintf(stderr, "usage: a.out <integer values>\n");
    return -1;
  }
  size = argc - 1;

  /* get the default attributes */
  pthread_attr_init(&attr);
  /* create the thread */
  pthread_create(&tid, &attr, runner, argv + 1);
  /* wait for the thread to exit */
  pthread_join(tid, NULL);

  printf("\nsum = %d\n", max);
}

/* The thread will begin control in this function */
void *runner(void *param) {
  int i, j;
  char buffer[10] = {0};
  // int *arr = (int*)malloc(size * sizeof(int));
  char c;
  max = -1;

  for (i = 0; i < size; i++) {
    j = 0;
    do {
      c = ((char **)param)[i][j];
      buffer[j] = c;
      j++;
    } while (c != '\0');
    int num = atoi(buffer);

    if (i == 0) {
      max = num;
    } else if (max < num) {
      max = num;
    }
  }
  // free(arr);
  pthread_exit(0);
}

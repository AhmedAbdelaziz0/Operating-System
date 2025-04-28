#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>

#define RESET "\033[0m"
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#define WHITE "\033[37m"   /* White */

typedef int buffer_item;
#define BUFFER_SIZE 5
struct Buffer {
  buffer_item data[BUFFER_SIZE];
  int front;
  int rear;
  int count;
} buffer;

pthread_mutex_t mutex;
sem_t empty, full;

void *producer(void *param);
void *consumer(void *param);
int insert_item(buffer_item item);
int remove_item(buffer_item *item);

int main(int argc, char *argv[]) {
  // init buffer
  buffer.front = 0;
  buffer.rear = 0;
  buffer.count = 0;

  if (argc != 4) {
    printf("Usage: %s <sleep time before exit> <producer count> <consumer "
           "count>\n",
           argv[0]);
    return 1;
  }

  int sleep_time = atoi(argv[1]);
  int producer_count = atoi(argv[2]);
  int consumer_count = atoi(argv[3]);

  // init mutex and semaphores
  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);

  // thread ids
  pthread_t producer_threads[producer_count];
  pthread_t consumer_threads[consumer_count];

  // create producers
  for (int i = 0; i < producer_count; i++) {
    pthread_create(&producer_threads[i], NULL, producer, NULL);
  }

  // create consumers
  for (int i = 0; i < consumer_count; i++) {
    pthread_create(&consumer_threads[i], NULL, consumer, NULL);
  }

  sleep(sleep_time);

  // TODO more graceful shutdown is needed
  // join threads
  for (int i = 0; i < producer_count; i++) {
    pthread_kill(producer_threads[i], SIGTERM); // signal termination
  }

  for (int i = 0; i < consumer_count; i++) {
    pthread_kill(consumer_threads[i], SIGTERM);
  }

  return 0;
}

int insert_item(buffer_item item) {
  if (buffer.count == BUFFER_SIZE) {
    return -1;
  }

  buffer.data[buffer.rear] = item;
  buffer.rear = (buffer.rear + 1) % BUFFER_SIZE;
  buffer.count++;
  return 0;
}

int remove_item(buffer_item *item) {
  if (buffer.count == 0) {
    return -1;
  }

  *item = buffer.data[buffer.front];
  buffer.front = (buffer.front + 1) % BUFFER_SIZE;
  buffer.count--;
  return 0;
}

void *producer(void *param) {
  do {
    // assume we prepare the item instead of sleep
    sleep(rand() % 5 + 1);

    // insert the item
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    int item = rand();

    if (insert_item(item) == -1) {
      printf("%sBuffer is full.\n%s", RED, RESET);
    } else {
      printf("%sProduced %d\n%s", GREEN, item, RESET);
    }

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  } while (1);
}

void *consumer(void *param) {
  do {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    buffer_item item;
    if (remove_item(&item) == -1) {
      printf("Buffer is empty.\n");
    } else {
      printf("%sConsumed %d\n%s", BLUE, item, RESET);
    }

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    // assume we consume the item
    sleep(rand() % 5 + 1);
  } while (1);
}

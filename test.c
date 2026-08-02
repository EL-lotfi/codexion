// #include "augmente_count.c"

// void augmente_count();
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int count = 0;
pthread_mutex_t mutex;
void *routine(void *arg)
{
  // pthread_mutex_lock(&mutex);
  count += 1;
  printf("the count is %d:\n", count);
  // pthread_mutex_unlock(&mutex);
}

int main()
{
  pthread_t       thread_1;
  pthread_t       thread_2;
  pthread_t       thread_3;
  pthread_t       thread_4;
  pthread_t       thread_5;
  pthread_mutex_init(&mutex, NULL);
  pthread_create(&thread_1, NULL, routine, NULL);
  pthread_create(&thread_2, NULL, routine, NULL);
  pthread_create(&thread_3, NULL, routine, NULL);
  pthread_create(&thread_4, NULL, routine, NULL);
  pthread_create(&thread_5, NULL, routine, NULL);
  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);
  pthread_join(thread_3, NULL);
  pthread_join(thread_4, NULL);
  pthread_join(thread_5, NULL);
}

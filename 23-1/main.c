#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <limits.h>

volatile unsigned int current_reader = 0;

unsigned int N;

pthread_mutex_t lock;

void* scan_value(void* args) {
  int my_id = *(int*)args;
  int value;
  int sum = 0;
  while (current_reader != N) {
    pthread_mutex_lock(&lock);
    if (current_reader != my_id) {
      pthread_mutex_unlock(&lock);
      continue;
    }
    if (scanf("%d", &value) == EOF) current_reader = N;
    else {
      sum += value;
      current_reader = (current_reader + 1) % N;
    }
    pthread_mutex_unlock(&lock);
  }
  return (void*)sum;
}

int main(int argc, char* argv[]) {
  pthread_mutex_init(&lock, NULL);
  N = strtol(argv[1], NULL, 10);
  pthread_t array_thread[N];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, 20000);
  unsigned int arr_id[N];
  for(int i = 0; i < N; i++) {
    arr_id[i] = i;
    pthread_create(&array_thread[i], &attr, &scan_value, (void*)&arr_id[i]);
  }
  int sum = 0;
  for(int i = 0; i < N; i++) {
    void* retval;
    pthread_join(array_thread[i], &retval);
    sum += (int)retval;
  }
  printf("%d", sum);
  return 0;
}

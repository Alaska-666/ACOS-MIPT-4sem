#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdlib.h>

typedef struct {
        unsigned int k;
        unsigned int N;
        unsigned int i;
        pthread_mutex_t* mutex;
        double* arr;
} args_t;

void* modify(void* args) {
  args_t arg = *(args_t*)args;
  while(arg.N > 0) {
    pthread_mutex_lock(arg.mutex);
    arg.arr[arg.i] += 1;
    if (arg.i == arg.k - 1) {
      arg.arr[0] += 1.01;
    } else {
      arg.arr[arg.i + 1] += 1.01;
    }
    if (arg.i == 0) {
      arg.arr[arg.k - 1] += 0.99;
    } else {
      arg.arr[arg.i - 1] += 0.99;
    }
    arg.N--;
    pthread_mutex_unlock(arg.mutex);
  }
}

int main(int argc, char* argv[]) {
  unsigned int N = atoi(argv[1]);
  unsigned int k = atoi(argv[2]);
  pthread_mutex_t lock;
  pthread_t t[k];
  pthread_mutex_init(&lock, NULL);
  double array[k];
  for(int i = 0; i < k; i++) array[k] = 0;
  args_t args[k];
  for(unsigned int i = 0; i < k; i++) {
    args[i].k = k;
    args[i].N = N;
    args[i].i = i;
    args[i].mutex = &lock;
    args[i].arr = (double*)array;
    pthread_create(&t[i], NULL, &modify, (void*)&args[i]);
  }
  for(unsigned int i = 0; i < k; i++) pthread_join(t[i], NULL);
  for(unsigned int i = 0; i < k; i++) printf("%.10g ", array[i]);
  return 0;
}

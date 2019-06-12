#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  uint64_t A;
  uint64_t B;
  pthread_cond_t* c;
  int pipe;
  int* ready;
} args_t;

void* generate(void* args) {
  uint64_t A = ((args_t*)args)->A;
  uint64_t B = ((args_t*)args)->B;
  pthread_cond_t* c = ((args_t*)args)->c;
  int* ready = ((args_t*)args)->ready;
  int pipe = ((args_t*)args)->pipe;
  if (A == 1) A++;
  while (A <= B) {
    unsigned int flag = 1;
    for(int i = 2; i*i <= A; i++)
      if (A % i == 0) flag = 0;
    if (flag) {
      write(pipe, &A, sizeof(A));
      *ready += 1;
      pthread_cond_signal(c);
    }
    A++;
  }
}

int main(int argc, char* argv[]) {
  uint64_t A = atoll(argv[1]);
  uint64_t B = atoll(argv[2]);
  uint32_t N = atoi(argv[3]);
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  pthread_t t;
  pthread_cond_t c;
  pthread_cond_init(&c, NULL);
  int pipes[2];
  socketpair(PF_LOCAL, SOCK_STREAM, 0, pipes);
  int ready = 0;
  args_t args = { .c = &c, .A = A, .B = B, .pipe = pipes[1], .ready=&ready };
  pthread_create(&t, NULL, generate, (void*)&args);
  uint64_t value;
  for(int i = 0; i < N; i++) {
    while (!ready) {
      pthread_cond_wait(&c, &mutex);
    }
    ready -=1;
    read(pipes[0], &value, sizeof(value));
    printf("%llu ", value);
  }
  pthread_cond_destroy(&c);
  pthread_mutex_destroy(&mutex);
  return 0;
}

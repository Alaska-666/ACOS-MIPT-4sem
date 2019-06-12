#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

void* function_one(void* argv) {
  int file_descr = *(int*) argv;
  int value;
  read(file_descr, &value, sizeof(int));
  while (value < 101 && value != 0) {
    value -= 3;
    if (value != 0 && value < 101) {
      printf("%d ", value);
      fflush(stdout);
      write(file_descr, &value, sizeof(int));
      if (read(file_descr, &value, sizeof(int)) == 0) {
        close(file_descr);
        exit(0);
      }
    }
  }
  printf("%d ", value);
  fflush(stdout);
  close(file_descr);
  exit(0);
}

void* function_two(void* argv) {
  int file_descr = *(int*) argv;
  int value;
  read(file_descr, &value, sizeof(int));
  while (value < 101 && value != 0) {
    value += 5;
    if (value != 0 && value < 101) {
      printf("%d ", value);
      fflush(stdout);
      write(file_descr, &value, sizeof(int));
      if (read(file_descr, &value, sizeof(int)) == 0) {
        close(file_descr);
        exit(0);
      }
    }
  }
  printf("%d ", value);
  fflush(stdout);
  close(file_descr);
  exit(0);
}

int main(int argc, char* argv[]) {
  int value = atoi(argv[1]);
  int pipes[2];
  socketpair(PF_LOCAL, SOCK_STREAM, 0, pipes);
  pthread_t thread[2];
  write(pipes[1], &value, sizeof(int));
  pthread_create(&thread[0], NULL, function_one, (void*)&pipes[0]);
  pthread_create(&thread[1], NULL, function_two, (void*)&pipes[1]);
  pthread_join(thread[0], NULL);
  pthread_join(thread[1], NULL);
  return 0;
}

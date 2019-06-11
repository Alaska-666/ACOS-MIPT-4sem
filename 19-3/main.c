#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

typedef double (*function_t)(double);

double* pmap_process(function_t func, const double* in, size_t count) {
  double* memory = mmap(NULL, sizeof(double) * count, PROT_WRITE | PROT_READ,
    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  sem_t* semaphore = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  sem_init(semaphore, 1, 0);

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = SIG_IGN;
  action.sa_flags = SA_RESTART | SA_NOCLDWAIT;
  sigaction(SIGCHLD, &action, NULL);

  size_t N = get_nprocs();
  size_t size_part = (count - 1) / N + 1;
  for(size_t i = 0; i < N; ++i) {
    pid_t pid = fork();
    if (pid == 0) {
      for(size_t j = 0; j < size_part; ++j) {
        memory[i * size_part + j] = func(in[i * size_part + j]);
      }
      sem_post(semaphore);
      exit(0);
    }
  }
  for(size_t i = 0; i < N; ++i) sem_wait(semaphore);
  munmap(semaphore, sizeof(sem_t));
  return memory;
}

void pmap_free(double* ptr, size_t count) {
  munmap(ptr, sizeof(double) * count);
}

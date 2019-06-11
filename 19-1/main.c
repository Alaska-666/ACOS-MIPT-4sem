#include <dlfcn.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
  sem_t request_ready;
  sem_t response_ready;
  char func_name[20];
  double value;
  double result;
} shared_data_t;

int main(int argc, char *argv[]) {
  char* shared_name = "/diht79804alaska12344";

  //start initialization
  int link = shm_open(shared_name, O_RDWR | O_CREAT, 0642);
  ftruncate(link, sizeof(shared_data_t));
  unsigned int size_mmap = sizeof(shared_data_t);
  shared_data_t *st = mmap(NULL, size_mmap, PROT_WRITE | PROT_READ, MAP_SHARED, link, 0);
  close(link);
  sem_init(&st->response_ready, 1, 0);
  sem_init(&st->request_ready, 1, 0);
  //end initialization

  void *lib = dlopen(argv[1], RTLD_NOW);
  printf("%s\n", shared_name);
  fflush(stdout);

  while (true) {
    sem_wait(&st->request_ready);
    if (strlen(st->func_name) == 0) { 
      break;
    }
    double (*name_function) (double) = dlsym(lib, st->func_name);
    st->result = name_function(st->value);
    sem_post(&st->response_ready);
  }
  shm_unlink(shared_name);
  munmap(st, sizeof(shared_data_t));
  dlclose(lib);
  sem_destroy(&st->request_ready);
  sem_destroy(&st->response_ready);
  return 0;
}

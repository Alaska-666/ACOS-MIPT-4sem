#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

int main(int argc, char *argv[]) {
  char sem_name[NAME_MAX];
  char shm_name[NAME_MAX];
  int N = 0;
  if (argc > 1) strncpy(sem_name, argv[1], sizeof(sem_name));
  else scanf("%s", sem_name);
  if (argc > 2) strncpy(shm_name, argv[2], sizeof(shm_name));
  else scanf("%s", shm_name);
  if (argc > 3) N = strtol(argv[3], NULL, 10);
  else scanf("%d", &N);

  sem_t* sem = sem_open(sem_name, 0);
  sem_wait(sem);
  sem_close(sem);

  int shm_id = shm_open(shm_name, O_RDONLY, 0);
  size_t map_size = N * sizeof(int);
  int *array = mmap(NULL, map_size, PROT_READ, MAP_SHARED, shm_id, 0);
  close(shm_id);
  for (size_t i = 0; i < N; i++) printf("%d ", array[i]);
  fflush(stdout);
  munmap(array, map_size);
}

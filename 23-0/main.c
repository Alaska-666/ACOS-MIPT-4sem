#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* print_value(void* args) {
  int value = *(int*)args;
  printf("%d ", value);
  fflush(stdout);
}

void* scan_value(void* args) {
  int value;
  if (scanf("%d", &value) == EOF) return NULL;
  return (void*)value;
}

int main(int argc, char* argv[]) {
  pthread_t thread;
  int array[600];
  int N = 0;
  while (1) {
    pthread_create(&thread, NULL, &scan_value, 0);
    void* retval;
    pthread_join(thread, &retval);
    if (retval == NULL) break;
    array[N] = (int)retval;
    N++;
  }
  puts("");
  for(int i = N - 1; i >= 0; i--) {
    int value = array[i];
    pthread_create(&thread, NULL, &print_value, (void*)&value);
    pthread_join(thread, NULL);
  }
}

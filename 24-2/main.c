#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>

typedef struct Item {
  struct Item *next;
  int64_t value;
} item_t;

item_t* head = NULL;
item_t* _Atomic tail = NULL;

void* thread_func(void* args) {
  int64_t i = ((int64_t*)args)[0];
  int64_t k = ((int64_t*)args)[1];
  int64_t current = i * k;
  while (current != 0 && !tail) {
    sched_yield();
  }
  while (tail && tail->value + 1 != current) {
    sched_yield();
  }
  while (current < (i + 1)* k) {
    item_t *new_node = malloc(sizeof(item_t));
    new_node->value = current;
    new_node->next = NULL;
    if (!head) {
      head = new_node;
    }
    item_t* prev_tail = atomic_exchange(&tail, new_node);
    if (prev_tail) {
      prev_tail->next = new_node;
    }
    current++;
  }
}

int main(int argc, char* argv[]) {
  int64_t N = atoi(argv[1]);
  int64_t k = atoi(argv[2]);
  pthread_t threads[N];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, 20000);
  for(int64_t i = 0; i < N; i++) {
    int64_t *arg = malloc(sizeof(int64_t) * 2);
    arg[0] = i;
    arg[1] = k;
    pthread_create(&threads[i], &attr, &thread_func, (void*)arg);
  }
  for(int i = 0; i < N; i++) pthread_join(threads[i], NULL);
  while (head) {
    printf("%lld ", head->value);
    head = head->next;
  }
}

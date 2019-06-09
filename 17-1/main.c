#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t counter = 0;
volatile sig_atomic_t exit_ = 0;

void handle_usr1(int signum) {
  counter++;
}

void handle_usr2(int signum) {
  counter *= -1;
}

void handle_sigterm(int signum) {
  exit_ = 1;
}

int main() {
  struct sigaction action_usr1;
  memset(&action_usr1, 0, sizeof(action_usr1));
  action_usr1.sa_handler = handle_usr1;
  action_usr1.sa_flags = SA_RESTART;
  sigaction(SIGUSR1, &action_usr1, NULL);

  struct sigaction action_usr2;
  memset(&action_usr2, 0, sizeof(action_usr2));
  action_usr2.sa_handler = handle_usr2;
  action_usr2.sa_flags = SA_RESTART;
  sigaction(SIGUSR2, &action_usr2, NULL);

  struct sigaction action_term;
  memset(&action_term, 0, sizeof(action_term));
  action_term.sa_handler = handle_sigterm;
  action_term.sa_flags = SA_RESTART;
  sigaction(SIGTERM, &action_term, NULL);

  printf("%d\n", getpid());
  fflush(stdout);
  scanf("%d", &counter);

  while (!exit_) {
    pause();
    printf("%d ", counter);
    fflush(stdout);
  }
  return 0;
}

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

volatile sig_atomic_t timeout = 0;
pid_t child_pid;

void handle_alarm(int signum, siginfo_t *info, void *icontext) {
  if (info->si_pid != 0) return;
  timeout = 1;
  kill(child_pid, SIGTERM);
}

int main(int argc, char *argv[]) {
  struct sigaction action_alarm;
  memset(&action_alarm, 0, sizeof(action_alarm));
  action_alarm.sa_flags = SA_SIGINFO | SA_RESTART;
  action_alarm.sa_sigaction = handle_alarm;
  sigaction(SIGALRM, &action_alarm, NULL);

  alarm(atoi(argv[1]));
  int child_info;
  child_pid = fork();
  if (child_pid == 0) {
    execvp(argv[2], argv + 2);
  }
  else {
    waitpid(child_pid, &child_info, 0);
    if (WIFSIGNALED(child_info)) {
      if (!timeout) {
        printf("signaled");
        return 1;
      }
      else {
        printf("timeout");
        return 2;
      }
    }
  }
  printf("ok");
  return 0;
}

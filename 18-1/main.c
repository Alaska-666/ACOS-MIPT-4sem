#include <signal.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t adress = 0;
volatile sig_atomic_t value = 1;

void handle_sigrtmin(int signum, siginfo_t* info, void* ucontext) {
  adress = info->si_pid;
  value = info->si_value.sival_int;
}

int main() {
  struct sigaction action_sigrtmin;
  memset(&action_sigrtmin, 0, sizeof(action_sigrtmin));
  action_sigrtmin.sa_flags = SA_SIGINFO | SA_RESTART;
  action_sigrtmin.sa_sigaction = handle_sigrtmin;
  sigaction(SIGRTMIN, &action_sigrtmin, NULL);

  sigset_t block_mask;
  sigfillset(&block_mask);
  sigdelset(&block_mask, SIGRTMIN);
  sigprocmask(SIG_BLOCK, &block_mask, NULL);
  
  pause();
  while (value) {
    union sigval sig_value;
    memset(&sig_value, 0, sizeof(sig_value));
    sig_value.sival_int = value - 1;
    sigqueue(adress, SIGRTMIN, sig_value);
    pause();
  }
  return 0;
}

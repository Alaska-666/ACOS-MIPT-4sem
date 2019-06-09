#include <sys/signalfd.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

static void transfer(FILE* tread) {
  char buffer[4096];
  memset(buffer, 0, sizeof(buffer));
  fgets(buffer, sizeof(buffer), tread);
  fputs(buffer, stdout);
  fflush(stdout);
}

int main(int argc, char* argv[]) {
  FILE* files[argc - 1];
  for(int i = 1; i < argc; i++) {
    files[i - 1] = fopen(argv[i], "r");
  }
  sigset_t mask_to_block, mask_to_catch;
  sigfillset(&mask_to_block);
  sigemptyset(&mask_to_catch);
  for(int i = 0; i < argc; i++) {
    sigaddset(&mask_to_catch, SIGRTMIN + i);
  }
  sigprocmask(SIG_BLOCK, &mask_to_block, NULL);
  int fd = signalfd(-1, &mask_to_catch, 0);
  struct signalfd_siginfo si;
  int index;
  while (1) {
    read(fd, &si, sizeof(si));
    index = si.ssi_signo - SIGRTMIN;
    if (0 == index) break;
    else transfer(files[index - 1]);
  }
  close(fd);
  for (int i = 1; i < argc; i++) fclose(files[i - 1]);
  return 0;
}

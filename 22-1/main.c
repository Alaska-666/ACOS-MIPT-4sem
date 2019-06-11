#include <sys/epoll.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <ctype.h>
#include <signal.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


volatile sig_atomic_t should_exit = false;

void handler_for_exit(int num) {
  should_exit = true;
}

int main(int argc, char* argv[]) {
  int fd = epoll_create1(0);
  struct sockaddr_in sock_addr = { .sin_family = AF_INET, .sin_port = htons(atoi(argv[1])), .sin_addr.s_addr = inet_addr("127.0.0.1") };
  int server = socket(AF_INET, SOCK_STREAM, 0);
  struct sigaction action_exit = { .sa_handler = handler_for_exit, .sa_flags = SA_RESTART };
  sigaction(SIGTERM, &action_exit, NULL);
  bind(server, (struct sockaddr*) &sock_addr, sizeof(sock_addr));
  fcntl(server, F_SETFL, fcntl(server, F_GETFL, 0) | O_NONBLOCK);
  listen(server, 128);
  struct epoll_event action = { .data.fd = server, .events = EPOLLET | EPOLLIN };
  struct epoll_event *actions;
  actions = calloc(32000, sizeof(action));
  epoll_ctl(fd, EPOLL_CTL_ADD, server, &action);
  while(true) {
    if (should_exit) break;
    unsigned int size_ = epoll_wait(fd, actions, 32000, -1);
    if (should_exit) break;
    for(unsigned int j = 0; j < size_; ++j)
      if (actions[j].data.fd != server) {
        char data[400];
        int tmp = 0;
        while((tmp = read(actions[j].data.fd, data, sizeof(data))) && tmp != -1) {
          for(unsigned int i = 0; i < tmp; ++i) data[i] = toupper(data[i]);
          write(actions[j].data.fd, data, tmp);
        }
        if (tmp == -1) continue;
        close(actions[j].data.fd);
      } else {
        struct sockaddr addr;
        socklen_t lenght = sizeof(addr);
        int new_fd = accept(server, &addr, &lenght);
        fcntl(new_fd, F_SETFL, fcntl(new_fd, F_GETFL, 0) | O_NONBLOCK);
        struct epoll_event new_action = { .data.fd = new_fd, .events = EPOLLIN | EPOLLET };
        epoll_ctl(fd, EPOLL_CTL_ADD, new_fd, &new_action);
      }
  }
  close(server);
  free(actions);
}

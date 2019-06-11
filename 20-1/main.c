#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  struct protoent *tcp = getprotobyname("TCP");
  int socket_fd = socket(AF_INET, SOCK_STREAM, tcp->p_proto);
  struct addrinfo hints = {};
  struct addrinfo *answer = NULL;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(argv[1], argv[2], &hints, &answer);
  connect(socket_fd, answer->ai_addr, answer->ai_addrlen);

  ssize_t socket_closed = dprintf(socket_fd, "%s ", argv[3]);
  if (socket_closed == EPIPE) {
    return 0;
  }

  char record[32];
  socket_closed = read(socket_fd, record, sizeof(record));
  if (socket_closed == 0) {
    return 0;
  }

  unsigned int value = atoi(record);
  for (unsigned int j = 0; j <= value; ++j) {
    socket_closed = dprintf(socket_fd, "%u ", j);
    if (socket_closed == EPIPE) {
      return 0;
    }
  }

  socket_closed = read(socket_fd, record, sizeof(record));
  if (socket_closed == 0) {
    return 0;
  }
  record[socket_closed] = '\0';
  printf("%s", record);
  close(socket_fd);
  freeaddrinfo(answer);
  return 0;
}

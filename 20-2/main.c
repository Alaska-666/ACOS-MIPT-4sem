#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
  struct protoent *tcp = getprotobyname("TCP");
  int fd = socket(AF_INET, SOCK_STREAM, tcp->p_proto);
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(strtol(argv[1], NULL, 10));
  bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  listen(fd, 128);
  int sum = 0;
  unsigned int answer = 1;
  while (answer != 0) {
    int data = accept(fd, (struct sockaddr*) NULL, NULL);
    read(data, &answer, sizeof(answer));
    close(data);
    sum += (int)ntohl(answer);
  }
  close(fd);
  printf("%d\n", sum);
  fflush(stdout);
}

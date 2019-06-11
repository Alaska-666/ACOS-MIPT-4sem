#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  char buf[1024], node[sizeof(buf)], service[sizeof(buf)];
  while (fgets(buf, sizeof(buf), stdin)) {
    if (sscanf(buf, "%s%s", node, service) != 2) {
      fprintf(stderr, "failed to parse line\n");
      exit(1);
    }
    struct addrinfo hints = { .ai_family = AF_INET, .ai_socktype = SOCK_STREAM };
    struct addrinfo *result = NULL;
    int err = getaddrinfo(node, service, &hints, &result);
    if (err) printf("%s\n", gai_strerror(err));
    else {
      if (result) {
        const struct sockaddr_in *minp = NULL;
        for (struct addrinfo *p = result; p; p = p->ai_next) {
          const struct sockaddr_in *addr = (const struct sockaddr_in *) p->ai_addr;
          if (!minp) minp = addr;
          else if (ntohl(addr->sin_addr.s_addr) < ntohl(minp->sin_addr.s_addr)) minp = addr;
        }
        if (minp) printf("%s:%d\n", inet_ntoa(minp->sin_addr), ntohs(minp->sin_port));
      }
      freeaddrinfo(result);
    }
  }
}

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
	struct protoent *protocol = getprotobyname("UDP");
	int fd = socket(AF_INET, SOCK_DGRAM, protocol->p_proto);
	struct addrinfo hints = { .ai_family = AF_INET, .ai_socktype = SOCK_STREAM };
	struct addrinfo *result = NULL;
	getaddrinfo("localhost", argv[1], &hints, &result);
	connect(fd, result->ai_addr, result->ai_addrlen);
	int32_t tmp = -1;
	while(scanf("%d", &tmp) != EOF) {
		sendto(fd, &tmp, sizeof(tmp), MSG_CONFIRM, NULL, 0);
		recvfrom(fd, &tmp, sizeof(tmp), MSG_WAITALL, NULL, 0);
		printf("%d ", tmp);
		fflush(stdout);
	}
	return 0;
}

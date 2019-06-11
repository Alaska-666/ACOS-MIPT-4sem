#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


typedef unsigned char byte;

void add_lexems(byte* source, byte* dest) {
  unsigned int current_index = 0;
  char* lexem_end;
  char* lexem_start = source;
  while (1) {
    lexem_end = strchr(lexem_start, '.');
    if (lexem_end != NULL) lexem_end[0] = '\0';
    dest[current_index] = (byte)(strlen(lexem_start));
    current_index += 1;
    strcpy(&dest[current_index], lexem_start);
    current_index += (unsigned int)(strlen(lexem_start));
    if (lexem_end == NULL) break;
    lexem_start = (byte*)&lexem_end[1];
  }
}

int main() {
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("8.8.8.8");
  addr.sin_port = htons(53);
  unsigned int size = sizeof(addr);

  int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  byte str[456];
  byte ending[5] = { 0x00, 0x00, 0x01, 0x00, 0x01 };
  unsigned int len;
  while (EOF != scanf("%s", str)) {
    len = strlen(str);
    byte dns[574] = { (byte)(getpid()), (byte)(getpid()), 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    add_lexems((byte*)&str, (byte*)&dns[12]);
    memcpy((byte*)&dns[13 + len], (byte*)&ending, 5);
    sendto(socket_fd, &dns, 18 + len, 0, (struct sockaddr*) &addr, sizeof(addr));
    do recvfrom(socket_fd, &dns, 574, 0, (struct sockaddr*)&addr, &size);
    while (dns[0] != (byte)(getpid()) || dns[1] != (byte)(getpid()));
    printf("%d.%d.%d.%d\n", dns[30 + len], dns[len + 31], dns[len + 32], dns[len + 33]);
  }
  close(socket_fd);
  return 0;
}

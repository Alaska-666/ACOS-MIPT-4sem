#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	struct sigaction action_pipe;
	memset(&action_pipe, 0, sizeof(action_pipe));
	action_pipe.sa_handler = SIG_IGN;
	action_pipe.sa_flags = SA_RESTART;
	sigaction(SIGPIPE, &action_pipe, NULL);

  int max_num = strtol(argv[2], NULL, 10);
  char* fifo_name = argv[1];
	mkfifo(fifo_name, 0642);
	pid_t call;
	scanf("%d", &call);
	int info = kill(call, SIGHUP);
	int fifo = open(fifo_name, O_WRONLY);
	int opened;
	int counter = 0;
	char string_num[12];
	while (counter <= max_num) {
		int string_len = sprintf(string_num, "%d ", counter);
		opened = write(fifo, &string_num, string_len);
		if (opened <= 0) break;
		++counter;
	}
	printf("%d", counter);
	fflush(stdout);
	close(fifo);
	return 0;
}

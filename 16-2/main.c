#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	char* cmd1 = argv[1];
	char* cmd2 = argv[2];
	int pair[2];
	pipe(pair);
	pid_t pid = fork();
	if (pid < 0) return 1;
	else if (pid == 0) {
		dup2(pair[1], 1);
		close(pair[1]);
		system(cmd1);
	}
	else {
		pid = fork();
		if (pid < 0) return 1;
		else if (pid == 0) {
			close(pair[1]);
			dup2(pair[0], 0);
			close(pair[0]);
			system(cmd2);
		}
		else {
			close(pair[0]);
			close(pair[1]);
			waitpid(pid, 0, 0);
		}
	}
}

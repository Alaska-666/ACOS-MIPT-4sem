#include <unistd.h>
#include <stdlib.h>

void piped_sys(char* command, int pipe_in[2], int pipe_out[2]) {
	dup2(pipe_in[0], 0);
	close(pipe_in[0]);
	if (pipe_out[0] == -1) dup2(pipe_out[1], 1);
	else {
		pipe(pipe_out);
		dup2(pipe_out[1], 1);
		close(pipe_out[1]);
	}
	system(command);
}

int main(int argc, char* argv[]) {
	int stdout_pair[2] = {-1, dup(1)};
	int pair1[2];
	int pair2[2];
	piped_sys(argv[1], stdout_pair, pair1);
	for (int a = 2; a < argc - 1; a++) {
		if (a % 2 == 0) piped_sys(argv[a], pair1, pair2);
		else piped_sys(argv[a], pair2, pair1);
	}
	if ((argc - 1) % 2 == 0) piped_sys(argv[argc - 1], pair1, stdout_pair);
	else piped_sys(argv[argc - 1], pair2, stdout_pair);
	close(pair1[1]);
	close(pair1[0]);
	close(pair2[0]);
	close(pair2[1]);
	return 0;
}

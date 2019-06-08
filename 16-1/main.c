#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int pair[2];
	pipe(pair);
	pid_t pid = fork();
	if (pid == 0) {
		dup2(pair[1], 2);
		close(pair[1]);
		execlp("gcc", "gcc", argv[1], NULL);
	}
	else {
		close(pair[1]);
		int count_err = 0;
		int count_warn = 0;
		FILE* file;
		int num_last_error = 0;
		int num_last_warn = 0;
		char str[1000];
		file = fdopen(pair[0], "r");
		char* count_read_symbol = fgets(str, 1000, file);
		while (count_read_symbol > 0) {
			char* index;
			if ((index = strstr(str, argv[1])) != NULL) {
				index += strlen(argv[1]) + 1;
				int num_line = 0;
				for (; index[0] >= '0' && index[0] <= '9'; index++) {
					num_line = num_line * 10 + index[0] - '0';
				}
				if (strstr(index, "error") && num_line > num_last_error) {
					++count_err;
					num_last_error = num_line;
				}
				if (strstr(index, "warning") && num_line > num_last_warn) {
                                	++count_warn;
                                	num_last_warn = num_line;
                        	}
 			}
			count_read_symbol = fgets(str, 1000, file);
		}
		waitpid(pid, 0, 0);
		printf("%d %d", count_err, count_warn);
	}
	return 0;
}

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int count_err = 0;
	int count_warn = 0;
	FILE* file;
	char path[200];
	strcpy(path, "gcc 2> errors ");
	strcat(path, argv[1]);
	system(path);
	int num_last_error = 0;
	int num_last_warn = 0;
	char str[1000];
	file = fopen("errors", "r");
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
	printf("%d %d", count_err, count_warn);
	return 0;
}

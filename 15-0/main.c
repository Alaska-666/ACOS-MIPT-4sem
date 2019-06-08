#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	for(int i = 1; i < argc; ++i) {
		int counter = 0;
		int j = 0;
		int flag = 0;
		while (argv[i][j] != '\0') {
			if (flag == 0) {
				if (argv[i][j] == '0' || argv[i][j] == 'x') ++j;
				else {
					flag = 1;
					++j;
					++counter;
				}
			}
			else {
				++j;
				++counter;
			}
		}
		if (counter % 2 == 0) printf("%d ", (counter / 2));
		else printf("%d ", (counter / 2 + 1));
	}
}

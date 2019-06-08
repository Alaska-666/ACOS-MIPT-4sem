#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

// (fixed) Enum for storing added
enum automata { nowhere, in_string, in_comment, in_long_comment };

int main() {
	uint64_t count = 0;
	int symbol;
	int prev_symbol = 0;
	enum automata state = nowhere;
	symbol = getchar();
	while(symbol != EOF) {
		if (symbol == '\n' && prev_symbol == '\\') {
			symbol = getchar();
			continue;
		}
		if (state == nowhere) {
			if (symbol == '*' && prev_symbol == '/') {
				--count;
				state = in_long_comment;
			}
			if (symbol == '/' && prev_symbol == '/') {
                                --count;
                                state = in_comment;
                        }
		}
		if (prev_symbol == '*' && symbol == '/' && state == in_long_comment) {
			--count;
			state = nowhere;
		}
		if (symbol == '"' && state != in_comment && state != in_long_comment) {
			if (state == in_string && prev_symbol != '\\') state = nowhere;
			else if (state != in_string) state = in_string;
		}
		if (symbol == '\n' && state == in_comment) state = nowhere;
		prev_symbol = symbol;
		if (!isspace(symbol) && state != in_comment && state != in_long_comment) ++count;
		symbol = getchar();
	}
	printf("%lld ", count);
	return 0;
}

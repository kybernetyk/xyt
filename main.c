/**
 * A little tool to extract youtube urls from a text stream. I'm sure there's a perl one-liner for that but I'm too dumb for perl and C is my scripting language.
 * Usage: just feed it a text stream via stdin.
 * Example: curl https://8ch.net/cyber/res/32671.html | ./xyt | sort | uniq
 * bai
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//adds https:// to the beginning of every line it prints 
#define PREPEND_HTTPS 1

void handle_stream(char initial_c) {
	char url[512];
	char c = 0;
	ssize_t index = 1;
	ssize_t rlen = 0;

	memset(url, 0x00, sizeof(url));
	url[0] = initial_c;

	for (;;) {
		rlen = read(0, &c, sizeof(c));
		if (rlen <= 0) {
			break;
		}
		if (c == '"' || c == ' ' || 
				c == '>' || c == '<' || 
				c == '&' || c == '*' || 
				c == '+' || c == '%') {
			break;
		}
		if (index >= 512) {
			fprintf(stderr, "Error!\n");
			exit(33);
		}
		url[index++] = c;
	}
	url[index] = '\n';
	url[index+1] = 0x00;
	if (strstr(url, "youtube.com/watch") != NULL ||
			strstr(url, "youtu.be") != NULL) {
		if (PREPEND_HTTPS && strstr(url, "https://") == NULL) {
			write(1, "https://", strlen("https://"));
		}
		write(1, url, strlen(url));
	}
}

int main(int argc, char *argv[]) {
	char c = 0;
	ssize_t rlen = 0; 
	for (;;) {
		rlen = read(0, &c, sizeof(c));
		if (rlen <= 0) {
			break;
		}
		if (c  == 'y') {
			handle_stream(c);
		}
	}

	return 0;
}

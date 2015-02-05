#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	int c = EOF;
	if (argc == 1) {
		fprintf(stderr, "Usage: cleantxt <file>\n");
		return 1;
	}
	fp = strcmp(argv[1], "-")? fopen(argv[1], "rb") : stdin;
	if (fp == 0) {
		fprintf(stderr, "ERROR: fail to open the input file.\n");
		return 1;
	}
	while (!feof(fp)) {
		c = fgetc(fp);
		if (isgraph(c) || c == '\t' || c == '\n' || c == ' ')
			fputc(c, stdout);
	}
	if (c != '\n') fputc('\n', stdout);
	fclose(fp);
	return 0;
}

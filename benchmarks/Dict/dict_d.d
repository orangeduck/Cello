// This program segfaults at the end of the input file. I think this is a bug
// of GDC. I can use std.stream or std.c.stdio instead, but those modules are
// extremely slow. In the end, I decide to specify the number of lines at the
// command line. Pretty nasty.

import std.stdio, std.string;

void main(string[] args) {
	int N = 5000000;
	char[] buf;
	int[char[]] h;
	int max = 1, n = 0;
	if (args.length >= 2) N = atoi(args[1]);
	while (readln(stdin, buf)) {
		int current = ++h[buf];
		max = (current > max)? current : max;
		if (++n == N) {
			writef(h.length); writef("\t"); writefln(max);
		}
	}
	writef(h.length); writef("\t"); writefln(max);
}

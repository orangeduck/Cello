#include "Cello.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  BUF_SIZE = 0x10000
};

int main(int argc, char *argv[]) {
	var h = new(Table, String, Int);
	resize(h, 1500000);
	int max = 1;
	char *buf = malloc(BUF_SIZE);
	while (!feof(stdin)) {
		fgets(buf, BUF_SIZE, stdin);
		var key = $S(buf);
		if (mem(h, key)) {
			struct Int* v = get(h, key);
			v->val++;
			if (max < v->val) { max = v->val; }
		} else {
			set(h, key, $I(1));
		}
	}
	del(h);
	return 0;
}

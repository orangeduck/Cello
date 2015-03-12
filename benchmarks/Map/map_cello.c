#include "Cello.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  BUF_SIZE = 0x10000
};

int main(int argc, char *argv[]) {
	var m = new(Map, String, Int);
	struct Get* mget = instance(m, Get);
	int max = 1;
	char *buf = malloc(BUF_SIZE);
	while (!feof(stdin)) {
		fgets(buf, BUF_SIZE, stdin);
		var key = $S(buf);
		if (mget->mem(m, key)) {
			struct Int* v = mget->get(m, key);
			v->val++;
			if (max < v->val) { max = v->val; }
		} else {
			mget->set(m, key, $I(1));
		}
	}
	del(m);
	return 0;
}

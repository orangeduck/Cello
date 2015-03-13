#include <stdio.h>
#include "kbtree.h"

typedef struct {
  char *key;
  int count;
} elem_t;

#define elem_cmp(a, b) (strcmp((a).key, (b).key))
KBTREE_INIT(str, elem_t, elem_cmp)

#define BUF_SIZE 0x10000
#define BLOCK_SIZE 0x100000

int main(int argc, char *argv[])
{
	char *buf, **mem = 0;
	int ret, max = 1, block_end = 0, curr = 0;
	kbtree_t(str) *h;
	h = kb_init(str, KB_DEFAULT_SIZE);
	buf = malloc(BUF_SIZE); // string buffer
	mem = malloc(sizeof(void*));
	mem[0] = malloc(BLOCK_SIZE); // memory buffer to avoid memory fragments
	curr = block_end = 0;
	while (!feof(stdin)) {
		fgets(buf, BUF_SIZE, stdin);
		elem_t t = { buf, 1 };
		elem_t *p = kb_getp(str, h, &t);
		if (!p) {
		
			int l = strlen(buf) + 1;
			if (block_end + l > BLOCK_SIZE) {
				++curr; block_end = 0;
				mem = realloc(mem, (curr + 1) * sizeof(void*));
				mem[curr] = malloc(BLOCK_SIZE);
			}
			memcpy(mem[curr] + block_end, buf, l);
			
			t.key = mem[curr] + block_end;
			t.count = 1;
			kb_putp(str, h, &t);
			block_end += l;
		
		} else {
		  p->count++;
			if (p->count > max) max = p->count;
    }

	}
	//printf("%u\t%d\n", kh_size(h), max);
	for (ret = 0; ret <= curr; ++ret) free(mem[ret]);
	free(mem);
	kb_destroy(str, h);
	free(buf);
	return 0;
}

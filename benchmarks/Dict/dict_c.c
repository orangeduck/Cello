#include <stdio.h>
#include "khash.h"
KHASH_MAP_INIT_STR(str, int)

#define BUF_SIZE 0x10000
#define BLOCK_SIZE 0x100000

int main(int argc, char *argv[])
{
	char *buf, **mem = 0;
	int ret, max = 1, block_end = 0, curr = 0;
	khint_t k;
	khash_t(str) *h;
	buf = malloc(BUF_SIZE); // string buffer
	h = kh_init(str);
	mem = malloc(sizeof(void*));
	mem[0] = malloc(BLOCK_SIZE); // memory buffer to avoid memory fragments
	curr = block_end = 0;
	while (!feof(stdin)) {
		fgets(buf, BUF_SIZE, stdin);
		k = kh_put(str, h, buf, &ret);
		if (ret) { // absent
			int l = strlen(buf) + 1;
			if (block_end + l > BLOCK_SIZE) {
				++curr; block_end = 0;
				mem = realloc(mem, (curr + 1) * sizeof(void*));
				mem[curr] = malloc(BLOCK_SIZE);
			}
			memcpy(mem[curr] + block_end, buf, l);
			kh_key(h, k) = mem[curr] + block_end;
			block_end += l;
			kh_val(h, k) = 1;
		} else {
			++kh_val(h, k);
			if (kh_val(h, k) > max) max = kh_val(h, k);
		}
	}
	//printf("%u\t%d\n", kh_size(h), max);
	for (ret = 0; ret <= curr; ++ret) free(mem[ret]);
	free(mem);
	kh_destroy(str, h);
	free(buf);
	return 0;
}

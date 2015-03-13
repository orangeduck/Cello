#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <map>

using namespace std;

struct eqstr {
	inline bool operator()(const char *s1, const char *s2) const {
		return strcmp(s1, s2) == 0;
    }
};

typedef map<const char*, int, eqstr> strmap;

#define BUF_SIZE 0x10000
#define BLOCK_SIZE 0x100000

int main(int argc, char *argv[])
{
	char *buf;
	int ret, max = 1, block_end = 0, curr = 0;
	char **mem;
	strmap *m = new strmap;
	buf = (char*)malloc(BUF_SIZE); // buffer size
	mem = (char**)malloc(sizeof(void*));
	mem[0] = (char*)malloc(BLOCK_SIZE); // memory buffer to avoid memory fragments
	curr = block_end = 0;
	while (!feof(stdin)) {
		fgets(buf, BUF_SIZE, stdin);
		strmap::iterator p = m->find(buf);
		if (p == m->end()) {
			int l = strlen(buf) + 1;
			if (block_end + l > BLOCK_SIZE) {
				++curr; block_end = 0;
				mem = (char**)realloc(mem, (curr + 1) * sizeof(void*));
				mem[curr] = (char*)malloc(BLOCK_SIZE);
			}
			memcpy(mem[curr] + block_end, buf, l);
			m->insert(pair<const char*, int>(mem[curr] + block_end, 1));
			block_end += l;
		} else {
			++p->second;
			if (max < p->second) max = p->second;
		}
	}
	//printf("%u\t%d\n", h->size(), max);
	for (int i = 0; i <= curr; ++i) free(mem[i]);
	free(mem); free(buf);
	delete m;
	return 0;
}

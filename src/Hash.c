#include "Cello.h"

static const char* Hash_Name(void) {
  return "Hash";
}

/* TODO */
static const char* Hash_Brief(void) {
  return "";
}

/* TODO */
static const char* Hash_Description(void) {
  return "";
}

/* TODO */
static const char* Hash_Examples(void) {
  return "";
}

/* TODO */
static const char* Hash_Methods(void) {
  return "";
}

var Hash = Cello(Hash,
  Instance(Doc,
    Hash_Name, Hash_Brief, Hash_Description, Hash_Examples, Hash_Methods));

/* TODO: Make non-static */
static uint64_t seed = CELLO_MAGIC_NUM;

void hash_init(uint64_t s) {
  seed = s;
}

uint64_t hash_seed(void) {
  return seed;
}

uint64_t hash_data(var self, size_t size) {
  
  const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;
	const uint64_t * data = (const uint64_t *)self;
	const uint64_t * end = data + (size/8);
  
	uint64_t h = seed ^ (size * m);

	while (data != end) {
		uint64_t k = *data++;
		k *= m; 
		k ^= k >> r; 
		k *= m; 
		h ^= k;
		h *= m; 
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch (size & 7) {
    case 7: h ^= (uint64_t)(data2[6]) << 48;
    case 6: h ^= (uint64_t)(data2[5]) << 40;
    case 5: h ^= (uint64_t)(data2[4]) << 32;
    case 4: h ^= (uint64_t)(data2[3]) << 24;
    case 3: h ^= (uint64_t)(data2[2]) << 16;
    case 2: h ^= (uint64_t)(data2[1]) << 8;
    case 1: h ^= (uint64_t)(data2[0]);
            h *= m;
	};
  
	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;

}
    
uint64_t hash(var self) {
  
  struct Hash* h = instance(self, Hash);
  if (h and h->hash) {
    return h->hash(self);
  }
  
  return hash_data(self, size(self));
}


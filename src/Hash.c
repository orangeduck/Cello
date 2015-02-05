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
  Member(Doc,
    Hash_Name, Hash_Brief, Hash_Description, Hash_Examples, Hash_Methods));

static uint64_t seed = 0xCE110;

void hash_init(uint64_t s) {
  seed = s;
}

uint64_t hash_seed(void) {
  return seed;
}
    
uint64_t hash(var self) {

  if (not implements(self, Hash)) {
    return (uint64_t)(intptr_t)self;
  } else {
    return method(self, Hash, hash);
  }
}

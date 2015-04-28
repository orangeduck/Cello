#include "Cello.h"

static const char* Hash_Name(void) {
  return "Hash";
}

static const char* Hash_Brief(void) {
  return "Hashable";
}

static const char* Hash_Description(void) {
  return
    "The `Hash` class provides a mechanism for hashing an object. This hash "
    "value should remain the same across objects that are also considered "
    "equal by the `Cmp` class. For objects that are not considered equal this "
    "value should aim to be evenly distributed across integers."
    "\n\n"
    "This is not a cryptographic hash. It is used for various objects or "
    "data structures that require fast hashing such as the `Table` type. Due "
    "to this it should not be used for cryptography or security."
    "\n\n"
    "By default an object is hashed by using its raw memory in the "
    "Murmurhash algorithm. Due to the link between them it is recommended to "
    "only override `Hash` and `Cmp` in conjunction.";
}

static const char* Hash_Definition(void) {
  return
    "struct Hash {\n"
    "  uint64_t (*hash)(var);\n"
    "};\n";
}

static struct Example* Hash_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "show($I(hash($I(  1)))); /* 1   */\n"
      "show($I(hash($I(123)))); /* 123 */\n"
      "show($I(hash_data($I(123), size(Int))));\n"
      "\n"
      "show($I(hash($S(\"Hello\"))));  /* 511 */\n"
      "show($I(hash($S(\"There\"))));  /* 515 */\n"
      "show($I(hash($S(\"People\")))); /* 629 */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Hash_Methods(void) {
  
  static struct Method methods[] = {
    {
      "hash", 
      "uint64_t hash(var self);",
      "Get the hash value for the object `self`."
    }, {
      "hash_data", 
      "uint64_t hash_data(void* data, size_t size);",
      "Hash `size` bytes pointed to by `data` using Murmurhash."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Hash = Cello(Hash,
  Instance(Doc,
    Hash_Name,       Hash_Brief,    Hash_Description, 
    Hash_Definition, Hash_Examples, Hash_Methods));
    
uint64_t hash_data(void* data, size_t size) {
  
  const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;
	const uint64_t * d = (const uint64_t *)data;
	const uint64_t * end = d + (size/8);
  
	uint64_t h = 0xCe110 ^ (size * m);

	while (d != end) {
		uint64_t k = *d++;
		k *= m; 
		k ^= k >> r; 
		k *= m; 
		h ^= k;
		h *= m; 
	}

	const unsigned char * data2 = (const unsigned char*)d;

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
  
  return hash_data(self, size(type_of(self)));
  
}


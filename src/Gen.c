#include "Cello.h"

#define NN 312
#define MM 156
#define UM 0xFFFFFFFF80000000ULL
#define LM 0x7FFFFFFFULL
#define MATRIX_A 0xB5026F5AA96619E9ULL

/* TODO: Make non-static */
static uint64_t mt[NN]; 
static int mti=NN+1; 

void init_genrand64(uint64_t seed) {
  mt[0] = seed;
  for (mti=1; mti<NN; mti++) {
    mt[mti] = (6364136223846793005ULL * (mt[mti-1] ^ (mt[mti-1] >> 62)) + mti);
  }
}

/* generates a random number on [0, 2^64-1]-interval */
uint64_t genrand64_int64(void) {
  int i;
  uint64_t x;
  static uint64_t mag01[2]={0ULL, MATRIX_A};

  if (mti >= NN) {
      /* if init_genrand64() has not been called, */
      /* a default initial seed is used           */
      if (mti == NN+1) {
          init_genrand64(5489ULL); 
      }
          
      for (i=0;i<NN-MM;i++) {
          x = (mt[i]&UM)|(mt[i+1]&LM);
          mt[i] = mt[i+MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
      }
      for (;i<NN-1;i++) {
          x = (mt[i]&UM)|(mt[i+1]&LM);
          mt[i] = mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
      }
      x = (mt[NN-1]&UM)|(mt[0]&LM);
      mt[NN-1] = mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];

      mti = 0;
  }

  x = mt[mti++];

  x ^= (x >> 29) & 0x5555555555555555ULL;
  x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
  x ^= (x << 37) & 0xFFF7EEE000000000ULL;
  x ^= (x >> 43);

  return x;
}

/* generates a random number on [0, 2^63-1]-interval */
int64_t genrand64_int63(void) {
  return (int64_t)(genrand64_int64() >> 1);
}

/* generates a random number on [0,1]-real-interval */
double genrand64_real1(void) {
  return (genrand64_int64() >> 11) * (1.0/9007199254740991.0);
}

/* generates a random number on [0,1)-real-interval */
double genrand64_real2(void) {
  return (genrand64_int64() >> 11) * (1.0/9007199254740992.0);
}

/* generates a random number on (0,1)-real-interval */
double genrand64_real3(void) {
  return ((genrand64_int64() >> 12) + 0.5) * (1.0/4503599627370496.0);
}

void gen_seed(uint64_t seed) {
  init_genrand64(seed);
}

uint64_t gen_c_int(void) {
  return genrand64_int63();
}

double gen_c_float(void) {
  return genrand64_real2();
}

/* TODO */
static const char* Gen_Name(void) {
  return "Gen";
}

/* TODO */
static const char* Gen_Brief(void) {
  return "";
}

/* TODO */
static const char* Gen_Description(void) {
  return "";
}

/* TODO */
static const char* Gen_Examples(void) {
  return "";
}

/* TODO */
static const char* Gen_Methods(void) {
  return "";
}

var Gen = Cello(Gen,
  Instance(Doc,
    Gen_Name, Gen_Brief, Gen_Description, 
    Gen_Examples, Gen_Methods));
    
var gen(var type) {
  return type_method(type, Gen, gen);
}

var shrink(var self) {
  struct Gen* g = instance(self, Gen);
  if (g and g->shrink) {
    return g->shrink(self);
  } else {
    return NULL;
  }
}

bool check(var func, var name, var iterations, var types) {
  
  var fargs = new(Tuple);
  bool success = true;
  int64_t i = 0;
  
  while (success and i < c_int(iterations)) {
    
    foreach (t in types) { push(fargs, gen(t)); }
    
    if (not call_with(func, fargs)) {
      print("%s: Checked %i - Failure\n", name, iterations);
      print("Arguments: "); foreach (a in fargs) { print("%$ ", a); }
      print("\n");
      /* TODO: Shrinking */
      success = false;
    }
    
    foreach (a in fargs) { del(a); }
    clear(fargs);
    
    i++;
  }
  
  del(fargs);
  
  if (success) {
    print("%s: Checked %i - All Passed\n", name, iterations);
    return true;
  } else {
    return false;
  }
  
}

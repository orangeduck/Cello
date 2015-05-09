#include <stdlib.h>

static void create_objects(int depth) {
  
  void
  *i00=malloc(16), *i01=malloc(16), *i02=malloc(16), *i03=malloc(16), *i04=malloc(16),
  *i05=malloc(16), *i06=malloc(16), *i07=malloc(16), *i08=malloc(16), *i09=malloc(16),
  *i10=malloc(16), *i11=malloc(16), *i12=malloc(16), *i13=malloc(16), *i14=malloc(16),
  *i15=malloc(16), *i16=malloc(16), *i17=malloc(16), *i18=malloc(16), *i19=malloc(16),
  *i20=malloc(16), *i21=malloc(16), *i22=malloc(16), *i23=malloc(16), *i24=malloc(16),
  *i25=malloc(16), *i26=malloc(16), *i27=malloc(16), *i28=malloc(16), *i29=malloc(16),
  *i30=malloc(16), *i31=malloc(16), *i32=malloc(16), *i33=malloc(16), *i34=malloc(16);
  
  if (depth == 2) {
    return;
  }
  
  for (size_t i = 0; i < 10; i++) {
    create_objects(depth+1);
  }
  
  volatile int noinline = 1;
  if (noinline) {
    free(i00); free(i01); free(i02); free(i03); free(i04); free(i05); 
    free(i06); free(i07); free(i08); free(i09); free(i10); free(i11); 
    free(i12); free(i13); free(i14); free(i15); free(i16); free(i17); 
    free(i18); free(i19); free(i20); free(i21); free(i22); free(i23); 
    free(i24); free(i25); free(i26); free(i27); free(i28); free(i29); 
    free(i30); free(i31); free(i32); free(i34);
  }
  
}

int main(int argc, char** argv) {
  
  for (size_t i = 0; i < 100; i++) {
    create_objects(0);
  }
  
}

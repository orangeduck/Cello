#include <stdlib.h>

class Int {
  int64_t i;
  public:
    Int() : i(0) {}
};

static void create_objects(int depth) {
  
  Int
  *i00=new Int(), *i01=new Int(), *i02=new Int(), *i03=new Int(), *i04=new Int(),
  *i05=new Int(), *i06=new Int(), *i07=new Int(), *i08=new Int(), *i09=new Int(),
  *i10=new Int(), *i11=new Int(), *i12=new Int(), *i13=new Int(), *i14=new Int(),
  *i15=new Int(), *i16=new Int(), *i17=new Int(), *i18=new Int(), *i19=new Int(),
  *i20=new Int(), *i21=new Int(), *i22=new Int(), *i23=new Int(), *i24=new Int(), 
  *i25=new Int(), *i26=new Int(), *i27=new Int(), *i28=new Int(), *i29=new Int(),
  *i30=new Int(), *i31=new Int(), *i32=new Int(), *i33=new Int(), *i34=new Int();
  
  if (depth == 2) {
    return;
  }
  
  for (size_t i = 0; i < 10; i++) {
    create_objects(depth+1);
  }
  
  volatile int noinline = 1;
  if (noinline) {
    delete(i00); delete(i01); delete(i02); delete(i03); delete(i04); delete(i05); 
    delete(i06); delete(i07); delete(i08); delete(i09); delete(i10); delete(i11);
    delete(i12); delete(i13); delete(i14); delete(i15); delete(i16); delete(i17); 
    delete(i18); delete(i19); delete(i20); delete(i21); delete(i22); delete(i23); 
    delete(i24); delete(i25); delete(i26); delete(i27); delete(i28); delete(i29); 
    delete(i30); delete(i31); delete(i32); delete(i34);
  }
  
}

int main(int argc, char** argv) {
  
  for (size_t i = 0; i < 100; i++) {
    create_objects(0);
  }
  
}

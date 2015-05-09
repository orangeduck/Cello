#include "Cello.h"

static void create_objects(int depth) {
  
  var
  i00=new(Int), i01=new(Int), i02=new(Int), i03=new(Int), i04=new(Int),
  i05=new(Int), i06=new(Int), i07=new(Int), i08=new(Int), i09=new(Int), 
  i10=new(Int), i11=new(Int), i12=new(Int), i13=new(Int), i14=new(Int),
  i15=new(Int), i16=new(Int), i17=new(Int), i18=new(Int), i19=new(Int),
  i20=new(Int), i21=new(Int), i22=new(Int), i23=new(Int), i24=new(Int), 
  i25=new(Int), i26=new(Int), i27=new(Int), i28=new(Int), i29=new(Int),
  i30=new(Int), i31=new(Int), i32=new(Int), i33=new(Int), i34=new(Int);
  
  volatile int noinline = 0;
  if (noinline) {
    show(i00); show(i01); show(i02); show(i03); show(i04); show(i05); 
    show(i06); show(i07); show(i08); show(i09); show(i10); show(i11); 
    show(i12); show(i13); show(i14); show(i15); show(i16); show(i17); 
    show(i18); show(i19); show(i20); show(i21); show(i22); show(i23); 
    show(i24); show(i25); show(i26); show(i27); show(i28); show(i29); 
    show(i30); show(i31); show(i32); show(i34);
  }
  
  if (depth == 2) {
    return;
  }
  
  for (size_t i = 0; i < 10; i++) {
    create_objects(depth+1);
  }
  
}

int main(int argc, char** argv) {
  
  for (size_t i = 0; i < 100; i++) {
    create_objects(0);
  }
  
}

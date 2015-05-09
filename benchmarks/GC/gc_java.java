public final class gc_java {

    final public static class Int {
      private int value;
      Int(int x) {
        value = x;
      }
    }

    public static void create_objects(int depth) {
      
      Int i00=new Int(0); Int i01=new Int(0); 
      Int i02=new Int(0); Int i03=new Int(0); 
      Int i04=new Int(0); Int i05=new Int(0); 
      Int i06=new Int(0); Int i07=new Int(0); 
      Int i08=new Int(0); Int i09=new Int(0); 
      Int i10=new Int(0); Int i11=new Int(0); 
      Int i12=new Int(0); Int i13=new Int(0); 
      Int i14=new Int(0); Int i15=new Int(0); 
      Int i16=new Int(0); Int i17=new Int(0); 
      Int i18=new Int(0); Int i19=new Int(0);
      Int i20=new Int(0); Int i21=new Int(0); 
      Int i22=new Int(0); Int i23=new Int(0); 
      Int i24=new Int(0); Int i25=new Int(0); 
      Int i26=new Int(0); Int i27=new Int(0); 
      Int i28=new Int(0); Int i29=new Int(0);
      Int i30=new Int(0); Int i31=new Int(0); 
      Int i32=new Int(0); Int i33=new Int(0); 
      Int i34=new Int(0);
      
      if (depth == 2) {
        return;
      }
      
      for (int i = 0; i < 10; i++) {
        create_objects(depth+1);
      }
      
    }

    public static void main(String[] args) {
        for (int i=0; i<10000; ++i) {
           create_objects(0);
        }
    }
}



class Int:
    pass

def create_objects(depth):
  
    i00=Int(); i01=Int(); i02=Int(); i03=Int(); i04=Int();
    i05=Int(); i06=Int(); i07=Int(); i08=Int(); i09=Int(); 
    i10=Int(); i11=Int(); i12=Int(); i13=Int(); i14=Int();
    i15=Int(); i16=Int(); i17=Int(); i18=Int(); i19=Int();
    i20=Int(); i21=Int(); i22=Int(); i23=Int(); i24=Int(); 
    i25=Int(); i26=Int(); i27=Int(); i28=Int(); i29=Int();
    i30=Int(); i31=Int(); i32=Int(); i33=Int(); i34=Int();
    
    if depth == 2:
        return
    
    for i in range(10):
      create_objects(depth+1)


for i in range(10000):
    create_objects(0)
    

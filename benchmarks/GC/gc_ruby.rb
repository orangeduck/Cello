
class Int
end

def create_objects(depth)
  
    i00=Int.new; i01=Int.new; i02=Int.new; i03=Int.new; i04=Int.new;
    i05=Int.new; i06=Int.new; i07=Int.new; i08=Int.new; i09=Int.new; 
    i10=Int.new; i11=Int.new; i12=Int.new; i13=Int.new; i14=Int.new;
    i15=Int.new; i16=Int.new; i17=Int.new; i18=Int.new; i19=Int.new;
    i20=Int.new; i21=Int.new; i22=Int.new; i23=Int.new; i24=Int.new; 
    i25=Int.new; i26=Int.new; i27=Int.new; i28=Int.new; i29=Int.new;
    i30=Int.new; i31=Int.new; i32=Int.new; i33=Int.new; i34=Int.new;
  
    if depth == 2 then
        return
    end
  
    for i in 0..10
        create_objects(depth+1)
    end
end

for i in 0..10000
    create_objects(0)
end

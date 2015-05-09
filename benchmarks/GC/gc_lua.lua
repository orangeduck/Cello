
function Int () 
  return {val=0}
end

function create_objects(depth)
  
    local i00=Int(); local i01=Int(); local i02=Int(); local i03=Int(); local i04=Int();
    local i05=Int(); local i06=Int(); local i07=Int(); local i08=Int(); local i09=Int(); 
    local i10=Int(); local i11=Int(); local i12=Int(); local i13=Int(); local i14=Int();
    local i15=Int(); local i16=Int(); local i17=Int(); local i18=Int(); local i19=Int();
    local i20=Int(); local i21=Int(); local i22=Int(); local i23=Int(); local i24=Int(); 
    local i25=Int(); local i26=Int(); local i27=Int(); local i28=Int(); local i29=Int();
    local i30=Int(); local i31=Int(); local i32=Int(); local i33=Int(); local i34=Int();
  
    if (depth == 2) then
        return;
    end
  
    for i=0,10 do
      create_objects(depth+1)
    end
end

for i=0, 10000 do
    create_objects(0)
end


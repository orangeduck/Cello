
function Int() {
  return {val:0}
}

function create_objects(depth) {
  
    var i00=Int(); var i01=Int(); var i02=Int(); var i03=Int(); var i04=Int();
    var i05=Int(); var i06=Int(); var i07=Int(); var i08=Int(); var i09=Int(); 
    var i10=Int(); var i11=Int(); var i12=Int(); var i13=Int(); var i14=Int();
    var i15=Int(); var i16=Int(); var i17=Int(); var i18=Int(); var i19=Int();
    var i20=Int(); var i21=Int(); var i22=Int(); var i23=Int(); var i24=Int(); 
    var i25=Int(); var i26=Int(); var i27=Int(); var i28=Int(); var i29=Int();
    var i30=Int(); var i31=Int(); var i32=Int(); var i33=Int(); var i34=Int();
  
    if (depth == 2) {
        return;
    }
    
    for (var i = 0; i < 10; i++) {
        create_objects(depth+1);
    }
}

for (var i = 0; i < 10000; i++) {
    create_objects(0);
}
    

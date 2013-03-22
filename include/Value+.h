#ifndef ValuePlus_h
#define ValuePlus_h

/*
** == Value ==
**
**  Experimental feature which allows for
**  returning stack objects (like are created with $)
**
**  val(Int) return_int(void) {
**    var x = $(Int, 10);
**    return *x;
**  }
**
**  val(Int) return_int(void) {
**    val(Int) x = {Int, 10};
**    return x;
**  }
*/

#define val(T) T##Data

#endif
#include <Cello/Cello.h>

/* Define a normal C structure */
struct Point {
  float x, y;
};

/* Make it compatible with Cello */
var Point = Cello(Point);

int main(int argc, char** argv) {
  
  /* Create on Stack or Heap */
  var p0 = $(Point, 0.0, 1.0);
  var p1 = new(Point, $(Point, 0.0, 2.0));
  
  /* It can be shown, compared, hashed, etc...
  **
  ** p0: <'Point' At 0x000000000022FC58>
  ** p1: <'Point' At 0x00000000004C7CC8>
  ** cmp: 1
  ** hash: 2849275892l
  */ 
  print("p0: %$\np1: %$\ncmp: %i\nhash: %ul\n",
    p0, p1, $I(cmp(p0, p1)), $I(hash(p0)));
  
  /* And collected by the GC when out of scope */
  return 0;
}
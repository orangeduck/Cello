/*
** Example libCello Program
*/

#include "Cello.h"

int main(int argc, char** argv) {
  
  srand(53124213);
  
  var table = new(Table, String, Int,
    $S("test0"), $I(0),
    $S("test1"), $I(1));	
  
  for (int i = 0; i < 10000; i++) {
    char strspace[100];
    sprintf(strspace, "%i", rand() % 1000);
    set(table, $S(strspace), $I(rand()));
  }
  
  for (int i = 0; i < 10000; i++) {
    char strspace[100];
    sprintf(strspace, "%i", rand() % 1000);
    if (mem(table, $S(strspace))) {
      rem(table, $S(strspace));
    }
  }
  
  for (int i = 0; i < 10000; i++) {
    char strspace[100];
    sprintf(strspace, "%i", rand() % 1000);
    set(table, $S(strspace), $I(rand()));
  }
  
  del(table);
  
  return 0;
  
}


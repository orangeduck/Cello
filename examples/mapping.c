#include "Cello.h"

int main(int argc, char** argv) {

  /* First class function declaration */
  fun (print_safe, args) {

    /* Exceptions */
    try {
      println("%$", get(args, $I(0)));
    } catch (e in IOError) {
      println("IOError: %$", e);
    }

    return None;
  };

  /* Tuple is a simple stack based collection */
  var t = tuple($F(51.25), $I(21), $S("Hello"));

  /* Higher Order Functions */
  map(t, print_safe);

  return 0;
}


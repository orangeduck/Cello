#include "Cello.h"

static const char* Seq_Name(void) {
  return "Seq";
}

/* TODO */
static const char* Seq_Brief(void) {
  return "";
}

/* TODO */
static const char* Seq_Description(void) {
  return "";
}

/* TODO */
static const char* Seq_Examples(void) {
  return "";
}

/* TODO */
static const char* Seq_Methods(void) {
  return "";
}

var Seq = typedecl(Seq,
  typeclass(Doc,
    Seq_Name, Seq_Brief, Seq_Description, Seq_Examples, Seq_Methods));

void reverse(var self) { method(self, Group, reverse); }
void sort(var self) { method(self, Group, sort); }

var maximum(var self) {
  
  if (len(self) == 0) return None;

  var best = get(self, $(Int, 0));
  foreach(item  in self) {
    if_gt(item, best) {
      best = item;
    }
  }
  
  return best;
}

var minimum(var self) {
  
  if (len(self) == 0) return None;
  
  var best = get(self, $(Int, 0));
  foreach(item in self) {
    if_lt(item, best) {
      best = item;
    }
  }
  
  return best;
}


#include "Cello.h"

void start(var self) {
  method(self, Start, start);
}

void stop(var self) {
  method(self, Start, stop);
}

var running(var self) {
  return method(self, Start, running);
}

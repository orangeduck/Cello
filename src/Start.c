#include "Cello.h"

const char* Start_Name(void) {
  return "Start";
}

const char* Start_Brief(void) {
  return "Can be started or stopped";
}

const char* Start_Description(void) {
  return
    "The `Start` class can be implemented by types which provide an abstract "
    "notion of a started and stopped state. This can be real processes such "
    "as `Thread`, or something like `File` where the on/off correspond to "
    "if the file is open or not."
    "\n\n"
    "The main nicety of the `Start` class is that it allows use of the `with` "
    "macro which performs the `start` function at the opening of a scope block "
    "and the `stop` function at the end.";
}

/*
void start(var self);
void stop(var self);
bool running(var self);
*/

static struct DocMethod* Start_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "start", 
      "void start(var self);",
      "Start the object `self`."
    }, {
      "stop", 
      "void stop(var self);",
      "Stop the object `self`."
    }, {
      "running", 
      "bool running(var self);",
      "Check if the object `self` is running."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

/* TODO: Examples */

var Start = Cello(Start, Instance(Doc, 
  Start_Name, Start_Brief, Start_Description, NULL, Start_Methods));

void start(var self) {
  method(self, Start, start);
}

void stop(var self) {
  method(self, Start, stop);
}

bool running(var self) {
  return method(self, Start, running);
}

var start_in(var self) {
  struct Start* s = instance(self, Start);
  if (s and s->start) {
    s->start(self);
  }
  return self;
}

var stop_in(var self) {
  struct Start* s = instance(self, Start);
  if (s and s->stop) {
    s->stop(self);
  }
  return NULL;
}


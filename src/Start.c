#include "Cello.h"

const char* Start_Name(void) {
  return "Start";
}

/* TODO */
const char* Start_Brief(void) {
  return "";
}

/* TODO */
const char* Start_Description(void) {
  return "";
}

/* TODO */
const char* Start_Examples(void) {
  return "";
}

/* TODO */
const char* Start_Methods(void) {
  return "";
}

var Start = Cello(Start, Member(Doc, 
  Start_Name, Start_Brief, Start_Description, Start_Examples, Start_Methods));


void start(var self) {
  method(self, Start, start);
}

void stop(var self) {
  method(self, Start, stop);
}

var running(var self) {
  return method(self, Start, running);
}

var start_in(var self) {
  start(self);
  return self;
}

var stop_in(var self) {
  stop(self);
  return Terminal;
}


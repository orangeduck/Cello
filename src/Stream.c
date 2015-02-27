#include "Cello.h"

static const char* Stream_Name(void) {
  return "Stream";
}

/* TODO */
static const char* Stream_Brief(void) {
  return "";
}

/* TODO */
static const char* Stream_Description(void) {
  return "";
}

/* TODO */
static const char* Stream_Examples(void) {
  return "";
}

/* TODO */
static const char* Stream_Methods(void) {
  return "";
}

var Stream = Cello(Stream,
  Instance(Doc, 
    Stream_Name, Stream_Brief, Stream_Description,
    Stream_Examples, Stream_Methods));

var sopen(var self, var name, var access) {
  return method(self, Stream, sopen, name, access);
}

void sclose(var self) {
  method(self, Stream, sclose);
}

void sseek(var self, var pos, var origin) {
  method(self, Stream, sseek, pos, origin);
}

int stell(var self) {
  return method(self, Stream, stell);
}

void sflush(var self) {
  method(self, Stream, sflush);
}

var seof(var self) {
  return method(self, Stream, seof);
}

int sread(var self, var output, var size) {
  return method(self, Stream, sread, output, size);
}

int swrite(var self, var input, var size) {
  return method(self, Stream, swrite, input, size);
}

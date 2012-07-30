#ifndef FilePlus_h
#define FilePlus_h

#include <stdio.h>

#include "Prelude+.h"
#include "Type+.h"

/** Stream - file like object */

class {
  void (*open)(var,const char*,const char*);
  void (*close)(var);
  void (*seek)(var,int,int);
  int (*tell)(var);
  void (*flush)(var);
  bool (*eof)(var);
} Stream;


void open(var self, const char* name, const char* access);
void close(var self);
void seek(var self, int pos, int origin);
int tell(var self);
void flush(var self);
bool eof(var self);


module File;

data {
  var type;
  FILE* f;
} FileData;


/** File_New(var self); */
var File_New(var self, va_list* args);
var File_Delete(var self);

void File_Assign(var self, obj);
var File_Copy(var self);

void File_Open(var self, const char* filename, const char* access);
void File_Close(var self);

void File_Seek(var self, int pos, int origin);
int File_Tell(var self);
void File_Flush(var self);

bool File_EOF(var self);
int File_GetError(var self);

instance(File, New) = { File_New, File_Delete };
instance(File, Assign) = { File_Assign };
instance(File, Copy) = { File_Copy };
instance(File, Stream) = { File_Open, File_Close, File_Seek, File_Tell, File_Flush, File_EOF };

#endif

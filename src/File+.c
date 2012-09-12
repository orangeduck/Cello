#include "File+.h"

#include <assert.h>

var File = methods {
  methods_begin(File),
  method(File, New),
  method(File, Assign),
  method(File, With),
  method(File, Copy),
  method(File, Stream),
  method(File, Dict),
  methods_end(File),
};

var File_New(var self, va_list* args) {
  FileData* fd = cast(self, File);
  const char* filename = va_arg(*args, const char*);
  const char* access = va_arg(*args, const char*);
  fd->f = fopen(filename, access);
  return self;
}

var File_Delete(var self) {
  FileData* fd = cast(self, File);
  if (fd->f != NULL) { close(self); }
  return self;
}

void File_Assign(var self, var obj) {
  FileData* fd0 = cast(self, File);
  FileData* fd1 = cast(obj, File);
  fd0->f = fd1->f;
}

var File_Copy(var self) {
  FileData* fd = cast(self, File); 
  FileData* fnew = new(File);
  fnew->f = fd->f;
  return fnew;
}

var File_Open(var self, const char* filename, const char* access) {
  FileData* fd = cast(self, File);
  fd->f = fopen(filename, access);
  return self;
}

void File_Close(var self) {
  FileData* fd = cast(self, File);
  fclose(fd->f);
}

void File_Seek(var self, int pos, int origin) {
  FileData* fd = cast(self, File);
  fseek(fd->f, pos, origin);
}

int File_Tell(var self) {
  FileData* fd = cast(self, File);
  return ftell(fd->f);
}

void File_Flush(var self) {
  FileData* fd = cast(self, File);
  fflush(fd->f);
}

bool File_EOF(var self) {
  FileData* fd = cast(self, File);
  return feof(fd->f);
}

int File_GetError(var self) {
  FileData* fd = cast(self, File);
  return ferror(fd->f);
}

int File_Read(var self, void* output, int size) {
  FileData* fd = cast(self, File);
  return fread(output, size, 1, fd->f);
}

int File_Write(var self, void* input, int size) {
  FileData* fd = cast(self, File);
  return fwrite(input, size, 1, fd->f);
}

void File_Read_Data(var self, var output) {
  self = cast(self, File);
  parse_read(output, self);
}

void File_Write_Data(var self, var input) {
  self = cast(self, File);
  parse_write(input, self);
}

var File_Get(var self, var type) {
  self = cast(self, File);
  var ret = allocate(type);
  parse_read(ret, self);
  return ret;
}


void File_Put(var self, var type, var obj) {
  self = cast(self, File);
  parse_write(obj, self);
}




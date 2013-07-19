#include "Cello/File.h"

#include "Cello/Exception.h"
#include "Cello/Number.h"
#include "Cello/String.h"

#include <assert.h>

var File = methods {
  methods_begin(File),
  method(File, New),
  method(File, With),
  method(File, Stream),
  method(File, Dict),
  method(File, Format),
  methods_end(File),
};

var File_New(var self, va_list* args) {
  FileData* fd = cast(self, File);
  const char* filename = va_arg(*args, const char*);
  const char* access = va_arg(*args, const char*);
  open(self, filename, access);
  return self;
}

var File_Delete(var self) {
  FileData* fd = cast(self, File);
  if (fd->f != NULL) { close(self); }
  return self;
}

var File_Open(var self, const char* filename, const char* access) {
  FileData* fd = cast(self, File);
  
  if (fd->f != NULL) { close(self); }
  
  fd->f = fopen(filename, access);
  
  if (fd->f == NULL) {
    throw(IOError, "Could not open file: %s", $(String, filename));
  }
  
  return self;
}

void File_Close(var self) {
  FileData* fd = cast(self, File);
  int err = fclose(fd->f);
  
  if (err != 0) {
    throw(IOError, "Failed to close file: %i", $(Int, err));
  }
  
  fd->f = NULL;
}

void File_Seek(var self, int pos, int origin) {
  FileData* fd = cast(self, File);
  int err = fseek(fd->f, pos, origin);
  
  if (err != 0) {
    throw(IOError, "Failed to seek in file: %i", $(Int, err));
  }
  
}

int File_Tell(var self) {
  FileData* fd = cast(self, File);
  int i = ftell(fd->f); 
  
  if (i == -1) {
    throw(IOError, "Failed to tell file: %i", $(Int, i));
  }
  
  return i;
}

void File_Flush(var self) {
  FileData* fd = cast(self, File);
  int err = fflush(fd->f);
  
  if (err != 0) {
    throw(IOError, "Failed to flush file: %i", $(Int, err));
  }
  
}

bool File_EOF(var self) {
  FileData* fd = cast(self, File);
  return feof(fd->f);
}


int File_Read(var self, void* output, int size) {
  FileData* fd = cast(self, File);
  int num = fread(output, size, 1, fd->f);
  
  if (num == -1) {
    throw(IOError, "Failed to read from file: %i", $(Int, num));
    return num;
  }
  
  return num;
}

int File_Write(var self, void* input, int size) {
  FileData* fd = cast(self, File);
  int num = fwrite(input, size, 1, fd->f);
  
  if (num != 1 && size != 0) {
    throw(IOError, "Failed to write to file: %i", $(Int, num));
  }
  
  return num;
}

void File_Read_Data(var self, var output) {
  self = cast(self, File);
  serial_read(output, self);
}

void File_Write_Data(var self, var input) {
  self = cast(self, File);
  serial_write(input, self);
}

var File_Get(var self, var type) {
  self = cast(self, File);
  var ret = allocate(type);
  serial_read(ret, self);
  return ret;
}

void File_Put(var self, var type, var obj) {
  self = cast(self, File);
  serial_write(obj, self);
}

int File_Format_To(var self, int pos, const char* fmt, va_list va) {
  FileData* fd = cast(self, File);
  return vfprintf(fd->f, fmt, va);
}

int File_Format_From(var self, int pos, const char* fmt, va_list va) {
  FileData* fd = cast(self, File);
  return vfscanf(fd->f, fmt, va);
}


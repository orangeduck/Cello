#include "Cello.h"

static const char* File_Name(void) {
  return "Name";
}

/* TODO */
static const char* File_Brief(void) {
  return "";
}

/* TODO */
static const char* File_Description(void) {
  return "";
}

/* TODO */
static const char* File_Examples(void) {
  return "";
}

/* TODO */
static const char* File_Methods(void) {
  return "";
}

static var File_New(var self, var args) {
  struct File* f = self;
  f->file = None;
  sopen(self,
    get(args, $(Int, 0)),
    get(args, $(Int, 1)));
  return self;
}

static var File_Del(var self) {
  struct File* f = self;
  if (f->file isnt None) {
    sclose(self);
  }
  return self;
}

static size_t File_Size(void) {
  return sizeof(struct File);
}

static var File_Open(var self, var filename, var access) {
  struct File* f = self;
  
  if (f->file isnt None) {
    sclose(self);
  }
  
  f->file = fopen(c_str(filename), c_str(access));
  
  if (f->file is None) {
    throw(IOError, "Could not open file: %s", filename);
  }
  
  return self;
}

static void File_Close(var self) {
  struct File* f = self;
  
  int err = fclose(f->file);
  if (err != 0) {
    throw(IOError, "Failed to close file: %i", $(Int, err));
  }
  
  f->file = None;
}

static void File_Seek(var self, var pos, var origin) {
  struct File* f = self;
  
  int err = fseek(f->file, c_int(pos), c_int(origin));
  if (err != 0) {
    throw(IOError, "Failed to seek in file: %i", $(Int, err));
  }
  
}

static int File_Tell(var self) {
  struct File* f = self;
  
  int i = ftell(f->file);
  if (i == -1) {
    throw(IOError, "Failed to tell file: %i", $(Int, i));
  }
  
  return i;
}

static void File_Flush(var self) {
  struct File* f = self;
  
  int err = fflush(f->file);
  if (err != 0) {
    throw(IOError, "Failed to flush file: %i", $(Int, err));
  }
  
}

static var File_EOF(var self) {
  struct File* f = self;
  return bool_var(feof(f->file));
}

static int File_Read(var self, var output, var size) {
  struct File* f = self;
  
  int num = fread(output, c_int(size), 1, f->file);
  if (num == -1) {
    throw(IOError, "Failed to read from file: %i", $(Int, num));
    return num;
  }
  
  return num;
}

static int File_Write(var self, var input, var size) {
  struct File* f = self;
  
  int num = fwrite(input, c_int(size), 1, f->file);  
  if (num != 1 && c_int(size) != 0) {
    throw(IOError, "Failed to write to file: %i", $(Int, num));
  }
  
  return num;
}

static int File_Format_To(var self, int pos, const char* fmt, va_list va) {
  struct File* f = self;
  return vfprintf(f->file, fmt, va);
}

static int File_Format_From(var self, int pos, const char* fmt, va_list va) {
  struct File* f = self;
  return vfscanf(f->file, fmt, va);
}

var File = typedecl(File,
  typeclass(Doc,
    File_Name, File_Brief, File_Description, File_Examples, File_Methods),
  typeclass(New, File_New, File_Del, File_Size),
  typeclass(Begin, NULL, File_Close),
  typeclass(Stream,
    File_Open, File_Close, File_Seek, File_Tell,
    File_Flush, File_EOF, File_Read, File_Write),
  typeclass(Format, File_Format_To, File_Format_From));


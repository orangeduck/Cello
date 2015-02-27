#include "Cello.h"

static const char* File_Name(void) {
  return "File";
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

static var File_Open(var self, var filename, var access);
static void File_Close(var self);

static var File_New(var self, var args) {
  struct File* f = self;
  File_Open(self, get(args, $I(0)), get(args, $I(1)));
  return self;
}

static var File_Del(var self) {
  struct File* f = self;
  if (f->file isnt None) { File_Close(self); }
  return self;
}

static size_t File_Size(void) {
  return sizeof(struct File);
}

static var File_Open(var self, var filename, var access) {
  struct File* f = self;
  
  if (f->file isnt None) { File_Close(self); }
  
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
    throw(IOError, "Failed to close file: %i", $I(err));
  }
  
  f->file = None;
}

static void File_Seek(var self, var pos, var origin) {
  struct File* f = self;
  
  /* TODO: Check file isnt None, also for other functions */
  
  int err = fseek(f->file, (long)c_int(pos), (int)c_int(origin));
  if (err != 0) {
    throw(IOError, "Failed to seek in file: %i", $I(err));
  }
  
}

static int File_Tell(var self) {
  struct File* f = self;
  
  int i = ftell(f->file);
  if (i == -1) {
    throw(IOError, "Failed to tell file: %i", $I(i));
  }
  
  return i;
}

static void File_Flush(var self) {
  struct File* f = self;
  
  int err = fflush(f->file);
  if (err != 0) {
    throw(IOError, "Failed to flush file: %i", $I(err));
  }
  
}

static var File_EOF(var self) {
  struct File* f = self;
  return bool_var(feof(f->file));
}

static int File_Read(var self, var output, var size) {
  struct File* f = self;
  
  int num = (int)fread(output, c_int(size), 1, f->file);
  if (num == -1) {
    throw(IOError, "Failed to read from file: %i", $I(num));
    return num;
  }
  
  return num;
}

static int File_Write(var self, var input, var size) {
  struct File* f = self;
  
  int num = (int)fwrite(input, c_int(size), 1, f->file);
  if (num != 1 && c_int(size) != 0) {
    throw(IOError, "Failed to write to file: %i", $I(num));
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

var File = Cello(File,
  Member(Doc,
    File_Name, File_Brief, File_Description, File_Examples, File_Methods),
  Member(Size, File_Size),
  Member(New, File_New, File_Del),
  Member(Start, NULL, File_Close, NULL),
  Member(Stream,
    File_Open, File_Close, File_Seek, File_Tell,
    File_Flush, File_EOF, File_Read, File_Write),
  Member(Format, File_Format_To, File_Format_From));


static const char* Process_Name(void) {
  return "Process";
}

/* TODO */
static const char* Process_Brief(void) {
  return "";
}

/* TODO */
static const char* Process_Description(void) {
  return "";
}

/* TODO */
static const char* Process_Examples(void) {
  return "";
}

/* TODO */
static const char* Process_Methods(void) {
  return "";
}

static var Process_Open(var self, var filename, var access);
static void Process_Close(var self);

static var Process_New(var self, var args) {
  struct Process* p = self;
  p->proc = None;
  Process_Open(self, get(args, $I(0)), get(args, $I(1)));
  return self;
}

static var Process_Del(var self) {
  struct Process* p = self;
  if (p->proc isnt None) { Process_Close(self); }
  return self;
}

static size_t Process_Size(void) {
  return sizeof(struct Process);
}

static var Process_Open(var self, var filename, var access) {
  struct Process* p = self;
  
  if (p->proc isnt None) { Process_Close(self); }
  
  p->proc = popen(c_str(filename), c_str(access));
  
  if (p->proc is None) {
    throw(IOError, "Could not open process: %s", filename);
  }
  
  return self;
}

static void Process_Close(var self) {
  struct Process* p = self;
  
  int err = pclose(p->proc);
  if (err != 0) {
    throw(IOError, "Failed to close process: %i", $I(err));
  }
  
  p->proc = None;
}

static void Process_Seek(var self, var pos, var origin) {
  struct Process* p = self;
  
  int err = fseek(p->proc, (long)c_int(pos), (int)c_int(origin));
  if (err != 0) {
    throw(IOError, "Failed to seek in process: %i", $I(err));
  }
  
}

static int Process_Tell(var self) {
  struct Process* p = self;
  
  int i = ftell(p->proc);
  if (i == -1) {
    throw(IOError, "Failed to tell process: %i", $I(i));
  }
  
  return i;
}

static void Process_Flush(var self) {
  struct Process* p = self;
  
  int err = fflush(p->proc);
  if (err != 0) {
    throw(IOError, "Failed to flush process: %i", $I(err));
  }
  
}

static var Process_EOF(var self) {
  struct Process* p = self;
  return bool_var(feof(p->proc));
}

static int Process_Read(var self, var output, var size) {
  struct Process* p = self;
  
  int num = (int)fread(output, c_int(size), 1, p->proc);
  if (num == -1) {
    throw(IOError, "Failed to read from process: %i", $I(num));
    return num;
  }
  
  return num;
}

static int Process_Write(var self, var input, var size) {
  struct Process* p = self;
  
  int num = (int)fwrite(input, c_int(size), 1, p->proc);
  if (num != 1 && c_int(size) != 0) {
    throw(IOError, "Failed to write to process: %i", $I(num));
  }
  
  return num;
}

static int Process_Format_To(var self, int pos, const char* fmt, va_list va) {
  struct Process* p = self;
  return vfprintf(p->proc, fmt, va);
}

static int Process_Format_From(var self, int pos, const char* fmt, va_list va) {
  struct Process* p = self;
  return vfscanf(p->proc, fmt, va);
}

var Process = Cello(Process,
  Member(Doc,
    Process_Name, Process_Brief, Process_Description, 
    Process_Examples, Process_Methods),
  Member(Size, Process_Size),
  Member(New,  Process_New, Process_Del),
  Member(Start, NULL, Process_Close, NULL),
  Member(Stream,
    Process_Open,  Process_Close, Process_Seek, Process_Tell,
    Process_Flush, Process_EOF,   Process_Read, Process_Write),
  Member(Format, Process_Format_To, Process_Format_From));


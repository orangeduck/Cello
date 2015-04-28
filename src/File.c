#include "Cello.h"

static const char* Stream_Name(void) {
  return "Stream";
}

static const char* Stream_Brief(void) {
  return "File-like";

}

static const char* Stream_Description(void) {
  return 
    "The `Stream` class represents an abstract set of operations that can be "
    "performed on File-like objects.";
}

static struct DocExample* Stream_Examples(void) {
  
  static struct DocExample examples[] = {
    {
      "Usage",
      "var f = sopen($(File, NULL), $S(\"test.bin\"), $S(\"r\"));\n"
      "\n"
      "char c;\n"
      "while (!seof(f)) {\n"
      "  sread(f, &c, 1);\n"
      "  putc(c, stdout);\n"
      "}\n"
      "\n"
      "sclose(f);\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct DocMethod* Stream_Methods(void) {
  
  static struct DocMethod methods[] = {
    {
      "sopen", 
      "var sopen(var self, var resource, var options);",
      "Open the stream `self` with a given `resource` and `options`."
    }, {
      "sclose", 
      "void sclose(var self);",
      "Close the stream `self`."
    }, {
      "sseek", 
      "void sseek(var self, int64_t pos, int origin);",
      "Seek to the position `pos` from some `origin` in the stream `self`."
    }, {
      "stell", 
      "int64_t stell(var self);",
      "Return the current position of the stream `stell`."
    }, {
      "sflush", 
      "void sflush(var self);",
      "Flush the buffered contents of stream `self`."
    }, {
      "seof", 
      "bool seof(var self);",
      "Returns true if there is no more information in the stream."
    }, {
      "sread", 
      "size_t sread(var self, void* output, size_t size);",
      "Read `size` bytes from the stream `self` and write them to `output`."
    }, {
      "swrite", 
      "size_t swrite(var self, void* input, size_t size);",
      "Write `size` bytes to the stream `self` and read them from `input`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Stream = Cello(Stream,
  Instance(Doc, 
    Stream_Name, Stream_Brief, Stream_Description,
    Stream_Examples, Stream_Methods));

var sopen(var self, var resource, var options) {
  return method(self, Stream, sopen, resource, options);
}

void sclose(var self) {
  method(self, Stream, sclose);
}

void sseek(var self, int64_t pos, int origin) {
  method(self, Stream, sseek, pos, origin);
}

int64_t stell(var self) {
  return method(self, Stream, stell);
}

void sflush(var self) {
  method(self, Stream, sflush);
}

bool seof(var self) {
  return method(self, Stream, seof);
}

size_t sread(var self, void* output, size_t size) {
  return method(self, Stream, sread, output, size);
}

size_t swrite(var self, void* input, size_t size) {
  return method(self, Stream, swrite, input, size);
}

static const char* File_Name(void) {
  return "File";
}

static const char* File_Brief(void) {
  return "Operating System File";
}

static const char* File_Description(void) {
  return
    "The `File` type is a wrapper of the native C `FILE` type representing a "
    "file in the operating system.";
}

static struct DocExample* File_Examples(void) {
  
  static struct DocExample examples[] = {
    {
      "Usage",
      "var x = new(File, $S(\"test.bin\"), $S(\"wb\"));\n"
      "char* data = \"hello\";\n"
      "swrite(x, data, 5);\n"
      "sclose(x);\n"
    }, {
      "Formatted Printing",
      "var x = $(File, NULL);\n"
      "sopen(x, $S(\"test.txt\"), $S(\"w\"));\n"
      "print_to(x, 0, \"%$ is %$ \", $S(\"Dan\"), $I(23));\n"
      "print_to(x, 0, \"%$ is %$ \", $S(\"Chess\"), $I(24));\n"
      "sclose(x);\n"
    }, {
      "Automatic Closing",
      "with(f in new(File, $S(\"test.txt\"), $S(\"r\"))) {\n"
      "  var k = new(String); reserve(k, $I(100));\n"
      "  var v = new(Int, $I(0));\n"
      "  foreach (i in range($I(2))) {\n"
      "    scan_from(f, 0, \"%$ is %$ \", k, v);\n"
      "    show(k); show(v);\n"
      "  }\n"
      "}\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static var File_Open(var self, var filename, var access);
static void File_Close(var self);

static void File_New(var self, var args) {
  struct File* f = self;
  if (len(args) > 0) {
    File_Open(self, get(args, $I(0)), get(args, $I(1)));
  }
}

static void File_Del(var self) {
  struct File* f = self;
  if (f->file isnt NULL) { File_Close(self); }
}

static var File_Open(var self, var filename, var access) {
  struct File* f = self;
  
  if (f->file isnt NULL) { File_Close(self); }
  
  f->file = fopen(c_str(filename), c_str(access));
  
  if (f->file is NULL) {
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
  
  f->file = NULL;
}

static void File_Seek(var self, int64_t pos, int origin) {
  struct File* f = self;
  
  if (f->file is NULL) {
    throw(IOError, "Cannot seek file - no file open.");
  }
  
  int err = fseek(f->file, pos, origin);
  if (err != 0) {
    throw(IOError, "Failed to seek in file: %i", $I(err));
  }
  
}

static int64_t File_Tell(var self) {
  struct File* f = self;
  
  if (f->file is NULL) {
    throw(IOError, "Cannot tell file - no file open.");
  }
  
  int64_t i = ftell(f->file);
  if (i == -1) {
    throw(IOError, "Failed to tell file: %i", $I(i));
  }
  
  return i;
}

static void File_Flush(var self) {
  struct File* f = self;
  
  if (f->file is NULL) {
    throw(IOError, "Cannot flush file - no file open.");
  }
  
  int err = fflush(f->file);
  if (err != 0) {
    throw(IOError, "Failed to flush file: %i", $I(err));
  }
  
}

static bool File_EOF(var self) {
  struct File* f = self;
  
  if (f->file is NULL) {
    throw(IOError, "Cannot eof file - no file open.");
  }
  
  return feof(f->file);
}

static size_t File_Read(var self, void* output, size_t size) {
  struct File* f = self;
  
  if (f->file is NULL) {
    throw(IOError, "Cannot read file - no file open.");
  }
  
  size_t num = fread(output, size, 1, f->file);
  if (num isnt 1 and size isnt 0 and not feof(f->file)) {
    throw(IOError, "Failed to read from file: %i", $I(num));
    return num;
  }
  
  return num;
}

static size_t File_Write(var self, void* input, size_t size) {
  struct File* f = self;
  
  if (f->file is NULL) {
    throw(IOError, "Cannot write file - no file open.");
  }
  
  size_t num = fwrite(input, size, 1, f->file);
  if (num isnt 1 and size isnt 0) {
    throw(IOError, "Failed to write to file: %i", $I(num));
  }
  
  return num;
}

static int File_Format_To(var self, int pos, const char* fmt, va_list va) {
  struct File* f = self;
  
  if (f->file is NULL) {
    throw(IOError, "Cannot format to file - no file open.");
  }
  
  return vfprintf(f->file, fmt, va);
}

static int File_Format_From(var self, int pos, const char* fmt, va_list va) {
  struct File* f = self;
  
  if (f->file is NULL) {
    throw(IOError, "Cannot format from file - no file open.");
  }
  
  return vfscanf(f->file, fmt, va);
}

var File = Cello(File,
  Instance(Doc,
    File_Name, File_Brief, File_Description, File_Examples, NULL),
  Instance(New, File_New, File_Del),
  Instance(Start, NULL, File_Close, NULL),
  Instance(Stream,
    File_Open, File_Close, File_Seek, File_Tell,
    File_Flush, File_EOF, File_Read, File_Write),
  Instance(Format, File_Format_To, File_Format_From));


static const char* Process_Name(void) {
  return "Process";
}

static const char* Process_Brief(void) {
  return "Operating System Process";
}

static const char* Process_Description(void) {
  return 
    "The `Process` type is a wrapper for an operating system process as "
    "constructed by the unix-like call `popen`. In this sense it is much like "
    "a standard file in the operating system but that instead of writing data "
    "to a location you are writing it as input to a process.";
}

static struct DocExample* Process_Examples(void) {
  
  static struct DocExample examples[] = {
    {
      "Usage",
      "var x = new(Process, $S(\"ls\"), $S(\"r\"));\n"
      "var o = new(String);\n"
      "scan_from(x, 0, \"%s\", o);\n"
      "show(o);\n"
      "sclose(x);\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static var Process_Open(var self, var filename, var access);
static void Process_Close(var self);

static void Process_New(var self, var args) {
  struct Process* p = self;
  p->proc = NULL;
  Process_Open(self, get(args, $I(0)), get(args, $I(1)));
}

static void Process_Del(var self) {
  struct Process* p = self;
  if (p->proc isnt NULL) { Process_Close(self); }
}

static var Process_Open(var self, var filename, var access) {
  struct Process* p = self;
  
  if (p->proc isnt NULL) { Process_Close(self); }
  
  p->proc = popen(c_str(filename), c_str(access));
  
  if (p->proc is NULL) {
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
  
  p->proc = NULL;
}

static void Process_Seek(var self, int64_t pos, int origin) {
  struct Process* p = self;
  
  if (p->proc is NULL) {
    throw(IOError, "Cannot seek process - no process open.");
  }
  
  int err = fseek(p->proc, pos, origin);
  if (err != 0) {
    throw(IOError, "Failed to seek in process: %i", $I(err));
  }
  
}

static int64_t Process_Tell(var self) {
  struct Process* p = self;
  
  if (p->proc is NULL) {
    throw(IOError, "Cannot tell process - no process open.");
  }
  
  int64_t i = ftell(p->proc);
  if (i == -1) {
    throw(IOError, "Failed to tell process: %i", $I(i));
  }
  
  return i;
}

static void Process_Flush(var self) {
  struct Process* p = self;
  
  if (p->proc is NULL) {
    throw(IOError, "Cannot flush process - no process open.");
  }
  
  int err = fflush(p->proc);
  if (err != 0) {
    throw(IOError, "Failed to flush process: %i", $I(err));
  }
  
}

static bool Process_EOF(var self) {
  struct Process* p = self;
  
  if (p->proc is NULL) {
    throw(IOError, "Cannot eof process - no process open.");
  }
  
  return feof(p->proc);
}

static size_t Process_Read(var self, void* output, size_t size) {
  struct Process* p = self;
  
  if (p->proc is NULL) {
    throw(IOError, "Cannot read process - no process open.");
  }
  
  size_t num = fread(output, size, 1, p->proc);
  if (num isnt 1 and size isnt 0) {
    throw(IOError, "Failed to read from process: %i", $I(num));
    return num;
  }
  
  return num;
}

static size_t Process_Write(var self, void* input, size_t size) {
  struct Process* p = self;
  
  if (p->proc is NULL) {
    throw(IOError, "Cannot write process - no process open.");
  }
  
  size_t num = fwrite(input, size, 1, p->proc);
  if (num isnt 1 and size isnt 0) {
    throw(IOError, "Failed to write to process: %i", $I(num));
  }
  
  return num;
}

static int Process_Format_To(var self, int pos, const char* fmt, va_list va) {
  struct Process* p = self;
  
  if (p->proc is NULL) {
    throw(IOError, "Cannot format to process - no process open.");
  }
  
  return vfprintf(p->proc, fmt, va);
}

static int Process_Format_From(var self, int pos, const char* fmt, va_list va) {
  struct Process* p = self;
  
  if (p->proc is NULL) {
    throw(IOError, "Cannot format from process - no process open.");
  }
  
  return vfscanf(p->proc, fmt, va);
}

var Process = Cello(Process,
  Instance(Doc,
    Process_Name, Process_Brief, Process_Description, 
    Process_Examples, NULL),
  Instance(New,  Process_New, Process_Del),
  Instance(Start, NULL, Process_Close, NULL),
  Instance(Stream,
    Process_Open,  Process_Close, Process_Seek, Process_Tell,
    Process_Flush, Process_EOF,   Process_Read, Process_Write),
  Instance(Format, Process_Format_To, Process_Format_From));


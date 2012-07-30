void open(var self, const char* name, const char* access) {
  Stream* istream = Type_Class(type_of(self), Stream)
  assert(istream->open);
  istreatm->open(self, name, access);
}

void close(var self) {
  Stream* istream = Type_Class(type_of(self), Stream)
  assert(istream->close);
  istreatm->close(self);
}

void seek(var self, int pos, int origin) {
  Stream* istream = Type_Class(type_of(self), Stream)
  assert(istream->seek);
  istreatm->seek(self, pos, origin);
}

int tell(var self) {
  Stream* istream = Type_Class(type_of(self), Stream)
  assert(istream->tell);
  return istreatm->tell(self);
}

void flush(var self) {
  Stream* istream = Type_Class(type_of(self), Stream)
  assert(istream->flush);
  istreatm->flush(self);
}

bool eof(var self) {
  Stream* istream = Type_Class(type_of(self), Stream)
  assert(istream->eof);
  return istreatm->eof(self);
}

var File = methods {
  methods_begin(File),
  method(File, New),
  method(File, Assign),
  method(File, Copy),
  method(File, Stream),
  methods_end(File),
};

var File_New(var self, va_list* args) {
  FileData* fd = cast(self, File);
  fd->f = NULL;
}

var File_Delete(var self) {
}

void File_Assign(var self, obj) {
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

void File_Open(var self, const char* filename, const char* access) {
  FileData* fd = cast(self, File);
  fopen(fd->f, filename, access);
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



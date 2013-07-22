/*
** == File ==
**
**  Basic file wrapper
*/

#ifndef CelloFile_h
#define CelloFile_h

#include "Prelude.h"
#include "Type.h"
#include "Format.h"

#include <stdio.h>

global var File;

data {
  var type;
  FILE* f;
} FileData;

/** File_New(var self, const char* filename, const char* access); */
var File_New(var self, va_list* args);
var File_Delete(var self);

var File_Open(var self, const char* filename, const char* access);
void File_Done(var self);

void File_Seek(var self, int pos, int origin);
int File_Tell(var self);
void File_Flush(var self);

bool File_EOF(var self);

int File_Read_From(var self, void* output, int size);
int File_Write_To(var self, void* input, int size);

void File_Read_Data(var self, var output);
void File_Write_Data(var self, var input);

var File_Get(var self, var type);
void File_Put(var self, var type, var obj);

int File_Format_To(var self, int pos, const char* fmt, va_list va);
int File_Format_From(var self, int pos, const char* fmt, va_list va);

instance(File, New) = { sizeof(FileData), File_New, File_Delete };
instance(File, With) = { NULL, File_Done };
instance(File, Stream) = { File_Open, File_Done, File_Seek, File_Tell, File_Flush, File_EOF, File_Read_From, File_Write_To };
instance(File, Dict) = { File_Get, File_Put };
instance(File, Format) = { File_Format_To, File_Format_From };

#endif

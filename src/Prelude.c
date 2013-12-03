#include "Cello/Prelude.h"

#include "Cello/Type.h"
#include "Cello/Bool.h"
#include "Cello/None.h"
#include "Cello/Exception.h"
#include "Cello/File.h"
#include "Cello/String.h"
#include "Cello/Number.h"
#include "Cello/Char.h"
#include "Cello/Reference.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

var Undefined = Singleton(Undefined);

/*
** The type_of a Type object is just "Type" again.
** But because "Type" is extern it isn't a constant expression.
** This means it cannot be set at compile time.
**
** So by convention at compile time the type_of a Type object is set to NULL.
** So if we access a struct and it tells us NULL is the type, assume "Type".
*/

var type_of(var self) {
  
  /* Test against Builtins */
  if (self is Undefined) return throw(ValueError, "Received 'Undefined' as value to 'type_of'");
  if (self is True) return Bool;
  if (self is False) return Bool;

  /* Use first entry in struct */
  var entry = ((ObjectData*)self)->type;
  if (entry is NULL) {
    return Type;
  } else {
    return entry;
  }

}

size_t size(var type) {
  return type_class_method(cast(type, Type), New, size);
}

var allocate(var type) {

  /* Allocate space for type, set type entry */
  type = cast(type, Type);
  
  var self;
  if (size(type) <= sizeof(ObjectData)) {
    self = NULL;
  } else {
    self = calloc(1, size(type));
    if (self == NULL) { throw(OutOfMemoryError, "Cannot create new '%s', out of memory!", type); }
    ((ObjectData*)self)->type = type;
  }
  
  return self;
}

void deallocate(var obj) {
  free(obj);
}

var new_vl(var type, var_list vl) { 
  
  var self = allocate(type);
  
  if (type_implements_method(type, New, construct_vl)) {
    self = type_class_method(type, New, construct_vl, self, vl);
  }
  
  return self;
}

void delete(var self) {
  
  if (type_implements_method(type_of(self), New, destruct)) {
    self = type_class_method(type_of(self), New, destruct, self);
  }
  
  deallocate(self);
}

var construct_vl(var self, var_list vl) {
  return type_class_method(type_of(self), New, construct_vl, self, vl);
}

var destruct(var self) {
  return type_class_method(type_of(self), New, destruct, self);
}

void assign(var self, var obj) {
  type_class_method(type_of(self), Assign, assign, self, obj);
}

var copy(var self) {
  return type_class_method(type_of(self), Copy, copy, self);
}

var eq(var lhs, var rhs) {
  
  if (not type_implements(type_of(lhs), Eq)) {
    return bool_var(lhs == rhs);
  } else {
    return type_class_method(type_of(lhs), Eq, eq, lhs, rhs);
  }
  
}

var neq(var lhs, var rhs) {
  return bool_var(not eq(lhs, rhs));
}

var gt(var lhs, var rhs) {
  return type_class_method(type_of(lhs), Ord, gt, lhs, rhs);
}

var lt(var lhs, var rhs) {
  return type_class_method(type_of(lhs), Ord, lt, lhs, rhs);

}

var ge(var lhs, var rhs) {
  return bool_var(not lt(lhs, rhs));
}

var le(var lhs, var rhs) {
  return bool_var(not gt(lhs, rhs));
}

int len(var self) {
  return type_class_method(type_of(self), Collection, len, self);
}

var is_empty(var self) {
  return bool_var(len(self) == 0);
}

void clear(var self) {
  type_class_method(type_of(self), Collection, clear, self);
}

var contains(var self, var obj) {
  return type_class_method(type_of(self), Collection, contains, self, obj);
}

void discard(var self, var obj) {
  type_class_method(type_of(self), Collection, discard, self, obj);
}

var maximum(var self) {
  
  if (len(self) == 0) return None;

  var best = at(self, 0);
  foreach(item  in self) {
    if_gt(item, best) {
      best = item;
    }
  }
  
  return best;
}

var minimum(var self) {
  
  if (len(self) == 0) return None;
  
  var best = at(self, 0);
  foreach(item in self) {
    if_lt(item, best) {
      best = item;
    }
  }
  
  return best;
}

void reverse(var self) {
  type_class_method(type_of(self), Reverse, reverse, self);
}

void sort(var self) {
  type_class_method(type_of(self), Sort, sort, self);
}

void append(var self, var obj) {
  type_class_method(type_of(self), Append, append, self, obj);
}

var iter_start(var self) {
  return type_class_method(type_of(self), Iter, iter_start, self);
}

var iter_end(var self) {
  return type_class_method(type_of(self), Iter, iter_end, self);
}

var iter_next(var self, var curr) {
  return type_class_method(type_of(self), Iter, iter_next, self, curr);
}

var Iter_End = Singleton(Iter_End);

var at(var self, int index) {
  return type_class_method(type_of(self), At, at, self, index);
}

void set(var self, int index, var value) {
  type_class_method(type_of(self), At, set, self, index, value);
}

void push(var self, var val) {
  type_class_method(type_of(self), Push, push, self, val);
}

void push_at(var self, var val, int index) {
  type_class_method(type_of(self), Push, push_at, self, val, index);
}

void push_back(var self, var val) {
  type_class_method(type_of(self), Push, push_back, self, val);
}

void push_front(var self, var val) {
  type_class_method(type_of(self), Push, push_front, self, val);
}

var pop(var self) {
  return type_class_method(type_of(self), Push, pop, self);
}

var pop_at(var self, int index) {
  return type_class_method(type_of(self), Push, pop_at, self, index);
}

var pop_back(var self) {
  return type_class_method(type_of(self), Push, pop_back, self);
}

var pop_front(var self) {
  return type_class_method(type_of(self), Push, pop_front, self);
}

long hash(var self) {

  if (not type_implements(type_of(self), Hash)) {
    return (long)(intptr_t)self;
  } else {
    return type_class_method(type_of(self), Hash, hash, self);
  }
}

/*
** Given some number, `Hash_Table_Size` 
** returns a new number greater than 
** the old one, suitable for using 
** as the size for a hash table.
**
** To do this it picks from a table
** of primes or when exhausted multiplies
** the input by two.
*/

local const long Hash_Primes[20] = {
  23,      53,      101,     197,
  389,     683,     1259,    2417,
  4733,    9371,    18617,   37097,
  74093,   148073,  296099,  592019,
  1100009, 2200013, 4400021, 8800019
};

long Hash_Table_Size(long size) {
  for (int i = 0; i < 20; i++) {
    if (Hash_Primes[i] > size) { return Hash_Primes[i]; }
  }
  return size * 2;
}

var get(var self, var key) {
  return type_class_method(type_of(self), Dict, get, self, key);
}

void put(var self, var key, var val) {
  type_class_method(type_of(self), Dict, put, self, key, val);
}

char as_char(var self) {
  return type_class_method(type_of(self), AsChar, as_char, self);
}

const char* as_str(var self) {
  return type_class_method(type_of(self), AsStr, as_str, self);
}

long as_long(var self) {
  return type_class_method(type_of(self), AsLong, as_long, self);
}

double as_double(var self) {
  return type_class_method(type_of(self), AsDouble, as_double, self);
}

var stream_open(var self, const char* name, const char* access) {
  return type_class_method(type_of(self), Stream, stream_open, self, name, access);
}

void stream_close(var self) {
  type_class_method(type_of(self), Stream, stream_close, self);
}

void stream_seek(var self, int pos, int origin) {
  type_class_method(type_of(self), Stream, stream_seek, self, pos, origin);
}

int stream_tell(var self) {
  return type_class_method(type_of(self), Stream, stream_tell, self);
}

void stream_flush(var self) {
  type_class_method(type_of(self), Stream, stream_flush, self);
}

bool stream_eof(var self) {
  return type_class_method(type_of(self), Stream, stream_eof, self);
}

int stream_read(var self, void* output, int size) {
  return type_class_method(type_of(self), Stream, stream_read, self, output, size);
}

int stream_write(var self, void* input, int size) {
  return type_class_method(type_of(self), Stream, stream_write, self, input, size);
}

void serial_read(var self, var input) {
  type_class_method(type_of(self), Serialize, serial_read, self, input);
}

void serial_write(var self, var output) {
  type_class_method(type_of(self), Serialize, serial_write, self, output);
}

void enter_with(var self) {
  if (type_implements_method(type_of(self), With, enter)) {
    type_class_method(type_of(self), With, enter, self);
  }
}

void exit_with(var self) {
  if (type_implements_method(type_of(self), With, exit)) {
    type_class_method(type_of(self), With, exit, self);
  }
}

var enter_for(var self) {
  enter_with(self);
  return self;
}

var exit_for(var self) {
  exit_with(self);
  return Undefined;
}



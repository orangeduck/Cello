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
#include <string.h>
#include <assert.h>

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
  if (self is Undefined) return throw(ValueError, "Recieved 'Undefined' as value to 'type_of'");
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

/* Allocate space for type, set type entry */
var allocate(var type) {
  type = cast(type, Type);
  
  New* inew = type_class(type, New);
 
  var self;
  if (inew->size <= sizeof(ObjectData)) {
    self = NULL;
  } else {
    self = calloc(1, inew->size);
    if (self == NULL) { throw(OutOfMemoryError, "Cannot create new '%s', out of memory!", type); }
    ((ObjectData*)self)->type = type;
  }
  
  return self;
}

void deallocate(var obj) {
  free(obj);
}

var new(var type, ...) { 
  
  var self = allocate(type);
  
  New* inew = type_class(type, New);
  if (inew->construct) {
    va_list args;
    va_start(args, type);
    self = inew->construct(self, &args);
    va_end(args);
  }
  
  return self;
}

void delete(var self) {
  New* inew = type_class(type_of(self), New);
  
  if (inew->destruct) {
    self = inew->destruct(self);
  }
  
  deallocate(self);
}

var construct(var self, ...) {
  New* inew = type_class(type_of(self), New);
  assert(inew->construct);
  va_list args;
  va_start(args, self);
  self = inew->construct(self, &args);
  va_end(args);
  return self;
}

var destruct(var self) {
  New* inew = type_class(type_of(self), New);
  assert(inew->destruct); 
  return inew->destruct(self);
}

void assign(var self, var obj) {
  Assign* iassign = type_class(type_of(self), Assign);
  assert(iassign->assign);
  iassign->assign(self, obj);
}

var copy(var self) {
  Copy* icopy = type_class(type_of(self), Copy);
  assert(icopy->copy);
  return icopy->copy(self);
}

var eq(var lhs, var rhs) {
  
  if (not type_implements(type_of(lhs), Eq)) {
    return (var)(intptr_t)(lhs == rhs);
  }
  
  Eq* ieq = type_class(type_of(lhs), Eq);
  assert(ieq->eq);
  return ieq->eq(lhs, rhs);
  
}

var neq(var lhs, var rhs) {
  return (var)(intptr_t)(not eq(lhs, rhs));
}

var gt(var lhs, var rhs) {
  Ord* iord = type_class(type_of(lhs), Ord);
  assert(iord->gt);
  return iord->gt(lhs, rhs);
}

var lt(var lhs, var rhs) {
  Ord* iord = type_class(type_of(lhs), Ord);
  assert(iord->lt);
  return iord->lt(lhs, rhs);
}

var ge(var lhs, var rhs) {
  return (var)(intptr_t)(not lt(lhs, rhs));
}

var le(var lhs, var rhs) {
  return (var)(intptr_t)(not gt(lhs, rhs));
}

int len(var self) {
  Collection* icollection = type_class(type_of(self), Collection);
  assert(icollection->len);
  return icollection->len(self);
}

var is_empty(var self) {
  return (var)(intptr_t)(len(self) == 0);
}

void clear(var self) {
  Collection* icollection = type_class(type_of(self), Collection);
  assert(icollection->clear);
  icollection->clear(self);
}

var contains(var self, var obj) {
  Collection* icollection = type_class(type_of(self), Collection);
  assert(icollection->contains);
  return icollection->contains(self, obj);
}

void discard(var self, var obj) {
  Collection* icollection = type_class(type_of(self), Collection);
  assert(icollection->discard);
  icollection->discard(self, obj);
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
  Reverse* ireverse = type_class(type_of(self), Reverse);
  assert(ireverse->reverse);
  ireverse->reverse(self);
}

void sort(var self) {
  Sort* isort = type_class(type_of(self), Sort);
  assert(isort->sort);
  isort->sort(self);
}

void append(var self, var obj) {
  Append* iappend = type_class(type_of(self), Append);
  assert(iappend->append);
  iappend->append(self, obj);
}

var iter_start(var self) {
  Iter* iiter = type_class(type_of(self), Iter);
  assert(iiter->iter_start);
  return iiter->iter_start(self);
}

var iter_end(var self) {
  Iter* iiter = type_class(type_of(self), Iter);
  assert(iiter->iter_end);
  return iiter->iter_end(self);
}

var iter_next(var self, var curr) {
  Iter* iiter = type_class(type_of(self), Iter);
  assert(iiter->iter_next);
  return iiter->iter_next(self, curr);
}

var at(var self, int index) {
  At* iat = type_class(type_of(self), At);
  assert(iat->at);
  return iat->at(self, index);
}

void set(var self, int index, var value) {
  At* iat = type_class(type_of(self), At);
  assert(iat->set);
  iat->set(self, index, value);
}

void push(var self, var val) {
  Push* ipush = type_class(type_of(self), Push);
  assert(ipush->push);
  ipush->push(self, val);
}

void push_at(var self, var val, int index) {
  Push* ipush = type_class(type_of(self), Push);
  assert(ipush->push_at);
  ipush->push_at(self, val, index);
}

void push_back(var self, var val) {
  Push* ipush = type_class(type_of(self), Push);
  assert(ipush->push_back);
  ipush->push_back(self, val);
}

void push_front(var self, var val) {
  Push* ipush = type_class(type_of(self), Push);
  assert(ipush->push_front);
  ipush->push_front(self, val);
}

var pop(var self) {
  Push* ipush = type_class(type_of(self), Push);
  assert(ipush->pop);
  return ipush->pop(self);
}

var pop_at(var self, int index) {
  Push* ipush = type_class(type_of(self), Push);
  assert(ipush->pop_at);
  return ipush->pop_at(self, index);
}

var pop_back(var self) {
  Push* ipush = type_class(type_of(self), Push);
  assert(ipush->pop_back);
  return ipush->pop_back(self);
}

var pop_front(var self) {
  Push* ipush = type_class(type_of(self), Push);
  assert(ipush->pop_front);
  return ipush->pop_front(self);
}

long hash(var self) {

  if (not type_implements(type_of(self), Hash)) {
    return (long)(intptr_t)self;
  }
  
  Hash* ihash = type_class(type_of(self), Hash);
  assert(ihash->hash);
  return ihash->hash(self);
}

var get(var self, var key) {
  Dict* idict = type_class(type_of(self), Dict);
  assert(idict->get);
  return idict->get(self, key);
}

void put(var self, var key, var val) {
  Dict* idict = type_class(type_of(self), Dict);
  assert(idict->put);
  return idict->put(self, key, val);
}

char as_char(var self) {
  AsChar* iaschar = type_class(type_of(self), AsChar);
  assert(iaschar->as_char);
  return iaschar->as_char(self);
}

const char* as_str(var self) {
  AsStr* iasstr = type_class(type_of(self), AsStr);
  assert(iasstr->as_str);
  return iasstr->as_str(self);
}

long as_long(var self) {
  AsLong* iaslong = type_class(type_of(self), AsLong);
  assert(iaslong->as_long);
  return iaslong->as_long(self);
}

double as_double(var self) {
  AsDouble* iasdouble = type_class(type_of(self), AsDouble);
  assert(iasdouble->as_double);
  return iasdouble->as_double(self);
}

var open(var self, const char* name, const char* access) {
  Stream* istream = type_class(type_of(self), Stream);
  assert(istream->open);
  return istream->open(self, name, access);
}

void close(var self) {
  Stream* istream = type_class(type_of(self), Stream);
  assert(istream->close);
  istream->close(self);
}

void seek(var self, int pos, int origin) {
  Stream* istream = type_class(type_of(self), Stream);
  assert(istream->seek);
  istream->seek(self, pos, origin);
}

int tell(var self) {
  Stream* istream = type_class(type_of(self), Stream);
  assert(istream->tell);
  return istream->tell(self);
}

void flush(var self) {
  Stream* istream = type_class(type_of(self), Stream);
  assert(istream->flush);
  istream->flush(self);
}

bool eof(var self) {
  Stream* istream = type_class(type_of(self), Stream);
  assert(istream->eof);
  return istream->eof(self);
}

int read(var self, void* output, int size) {
  Stream* istream = type_class(type_of(self), Stream);
  assert(istream->read);
  return istream->read(self, output, size);
}

int write(var self, void* input, int size) {
  Stream* istream = type_class(type_of(self), Stream);
  assert(istream->write);
  return istream->write(self, input, size);
}

void serial_read(var self, var input) {
  Serialize* iserialize = type_class(type_of(self), Serialize);
  assert(iserialize->serial_read);
  iserialize->serial_read(self, input);
}

void serial_write(var self, var output) {
  Serialize* iserialize = type_class(type_of(self), Serialize);
  assert(iserialize->serial_write);
  iserialize->serial_write(self, output);
}

void enter_with(var self) {
  With* iwith = type_class(type_of(self), With);
  if(iwith->enter) {
    iwith->enter(self);
  }
}

void exit_with(var self) {
  With* iwith = type_class(type_of(self), With);
  if(iwith->exit) {
    iwith->exit(self);
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



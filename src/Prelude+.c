#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Prelude+.h"
#include "Type+.h"

/*
** The type_of a Type object is just "Type" again.
** But because "Type" is extern it isn't a constant expression.
** This means the type_of a Type object cannot be set at compile time.
**
** So by convention at compile time the type_of a Type object is set to NULL.
** So if we access a struct and it tells us NULL is the type, just assume "Type".
*/

var type_of(var self) {
  var listed = ((ObjectData*)self)->type;
  
  if (listed is NULL) {
    return Type;
  } else {
    return listed;
  }
}

var allocate(var type) {
  type = cast(type, Type);
  
  New* inew = Type_Class(type, New);
 
  var self;
  if (inew->size <= sizeof(ObjectData)) {
    self = NULL;
  } else {
    self = calloc(1, inew->size);
    assert(self);
    ((ObjectData*)self)->type = type;
  }
  
  return self;
}

var new(var type, ...) { 
  
  var self = allocate(type);
  
  New* inew = Type_Class(type, New);
  if (inew->construct) {
    va_list args;
    va_start(args, type);
    self = inew->construct(self, &args);
    va_end(args);
  }
  
  return self;
}

void delete(var self) {
  New* inew = Type_Class(type_of(self), New);
  
  if (inew->destruct) {
    self = inew->destruct(self);
  }
  
  free(self);
}

var construct(var self, ...) {
  New* inew = Type_Class(type_of(self), New);
  assert(inew->construct);
  va_list args;
  va_start(args, self);
  self = inew->construct(self, &args);
  va_end(args);
  return self;
}

var destruct(var self) {
  New* inew = Type_Class(type_of(self), New);
  assert(inew->destruct); 
  return inew->destruct(self);
}

void assign(var self, var obj) {
  Assign* iassign = Type_Class(type_of(self), Assign);
  assert(iassign->assign);
  iassign->assign(self, obj);
}

var copy(var self) {
  Copy* icopy = Type_Class(type_of(self), Copy);
  assert(icopy->copy);
  return icopy->copy(self);
}

bool eq(var lhs, var rhs) {
  
  if (not Type_Implements(type_of(lhs), Eq)) {
    return lhs == rhs;
  }
  
  Eq* ieq = Type_Class(type_of(lhs), Eq);
  assert(ieq->eq);
  return ieq->eq(lhs, rhs);
  
}

bool neq(var lhs, var rhs) {
  return not eq(lhs, rhs);
}

bool gt(var lhs, var rhs) {
  Ord* iord = Type_Class(type_of(lhs), Ord);
  assert(iord->gt);
  return iord->gt(lhs, rhs);
}

bool lt(var lhs, var rhs) {
  Ord* iord = Type_Class(type_of(lhs), Ord);
  assert(iord->lt);
  return iord->lt(lhs, rhs);
}

bool ge(var lhs, var rhs) {
  return not lt(lhs, rhs);
}

bool le(var lhs, var rhs) {
  return not gt(lhs, rhs);
}

int len(var self) {
  Collection* icollection = Type_Class(type_of(self), Collection);
  assert(icollection->len);
  return icollection->len(self);
}

bool is_empty(var self) {
  Collection* icollection = Type_Class(type_of(self), Collection);
  assert(icollection->is_empty);
  return icollection->is_empty(self);
}

void clear(var self) {
  Collection* icollection = Type_Class(type_of(self), Collection);
  assert(icollection->clear);
  icollection->clear(self);
}

bool contains(var self, var obj) {
  Collection* icollection = Type_Class(type_of(self), Collection);
  assert(icollection->contains);
  return icollection->contains(self, obj);
}

void discard(var self, var obj) {
  Collection* icollection = Type_Class(type_of(self), Collection);
  assert(icollection->discard);
  icollection->discard(self, obj);
}

void reverse(var self) {
  Reverse* ireverse = Type_Class(type_of(self), Reverse);
  assert(ireverse->reverse);
  ireverse->reverse(self);
}

var iter_start(var self) {
  Iter* iiter = Type_Class(type_of(self), Iter);
  assert(iiter->iter_start);
  return iiter->iter_start(self);
}

var iter_end(var self) {
  Iter* iiter = Type_Class(type_of(self), Iter);
  assert(iiter->iter_end);
  return iiter->iter_end(self);
}

var iter_next(var self, var curr) {
  Iter* iiter = Type_Class(type_of(self), Iter);
  assert(iiter->iter_next);
  return iiter->iter_next(self, curr);
}

var at(var self, int index) {
  At* iat = Type_Class(type_of(self), At);
  assert(iat->at);
  return iat->at(self, index);
}

void set(var self, int index, var value) {
  At* iat = Type_Class(type_of(self), At);
  assert(iat->set);
  iat->set(self, index, value);
}

void push(var self, var val) {
  Push* ipush = Type_Class(type_of(self), Push);
  assert(ipush->push);
  ipush->push(self, val);
}

void push_at(var self, var val, int index) {
  Push* ipush = Type_Class(type_of(self), Push);
  assert(ipush->push_at);
  ipush->push_at(self, val, index);
}

void push_back(var self, var val) {
  Push* ipush = Type_Class(type_of(self), Push);
  assert(ipush->push_back);
  ipush->push_back(self, val);
}

void push_front(var self, var val) {
  Push* ipush = Type_Class(type_of(self), Push);
  assert(ipush->push_front);
  ipush->push_front(self, val);
}

var pop(var self) {
  Push* ipush = Type_Class(type_of(self), Push);
  assert(ipush->pop);
  return ipush->pop(self);
}

var pop_at(var self, int index) {
  Push* ipush = Type_Class(type_of(self), Push);
  assert(ipush->pop_at);
  return ipush->pop_at(self, index);
}

var pop_back(var self) {
  Push* ipush = Type_Class(type_of(self), Push);
  assert(ipush->pop_back);
  return ipush->pop_back(self);
}

var pop_front(var self) {
  Push* ipush = Type_Class(type_of(self), Push);
  assert(ipush->pop_front);
  return ipush->pop_front(self);
}

long hash(var self) {

  if (not Type_Implements(type_of(self), Hash)) {
    return (long)self;
  }
  
  Hash* ihash = Type_Class(type_of(self), Hash);
  assert(ihash->hash);
  return ihash->hash(self);
}

var get(var self, var key) {
  Dict* idict = Type_Class(type_of(self), Dict);
  assert(idict->get);
  return idict->get(self, key);
}

void put(var self, var key, var val) {
  Dict* idict = Type_Class(type_of(self), Dict);
  assert(idict->put);
  return idict->put(self, key, val);
}

char as_char(var self) {
  AsChar* iaschar = Type_Class(type_of(self), AsChar);
  assert(iaschar->as_char);
  return iaschar->as_char(self);
}

const char* as_str(var self) {
  AsStr* iasstr = Type_Class(type_of(self), AsStr);
  assert(iasstr->as_str);
  return iasstr->as_str(self);
}

long as_long(var self) {
  AsLong* iaslong = Type_Class(type_of(self), AsLong);
  assert(iaslong->as_long);
  return iaslong->as_long(self);
}

double as_double(var self) {
  AsDouble* iasdouble = Type_Class(type_of(self), AsDouble);
  assert(iasdouble->as_double);
  return iasdouble->as_double(self);
}

var open(var self, const char* name, const char* access) {
  Stream* istream = Type_Class(type_of(self), Stream);
  assert(istream->open);
  return istream->open(self, name, access);
}

void close(var self) {
  Stream* istream = Type_Class(type_of(self), Stream);
  assert(istream->close);
  istream->close(self);
}

void seek(var self, int pos, int origin) {
  Stream* istream = Type_Class(type_of(self), Stream);
  assert(istream->seek);
  istream->seek(self, pos, origin);
}

int tell(var self) {
  Stream* istream = Type_Class(type_of(self), Stream);
  assert(istream->tell);
  return istream->tell(self);
}

void flush(var self) {
  Stream* istream = Type_Class(type_of(self), Stream);
  assert(istream->flush);
  istream->flush(self);
}

bool eof(var self) {
  Stream* istream = Type_Class(type_of(self), Stream);
  assert(istream->eof);
  return istream->eof(self);
}

int read(var self, void* output, int size) {
  Stream* istream = Type_Class(type_of(self), Stream);
  assert(istream->read);
  return istream->read(self, output, size);
}

int write(var self, void* input, int size) {
  Stream* istream = Type_Class(type_of(self), Stream);
  assert(istream->write);
  return istream->write(self, input, size);
}

void parse_read(var self, var stream) {
  Parse* iparse = Type_Class(type_of(self), Parse);
  assert(iparse->parse_read);
  iparse->parse_read(self, stream);
}

void parse_write(var self, var stream) {
  Parse* iparse = Type_Class(type_of(self), Parse);
  assert(iparse->parse_write);
  iparse->parse_write(self, stream);
}

void enter_with(var self) {
  With* iwith = Type_Class(type_of(self), With);
  if(iwith->enter) {
    iwith->enter(self);
  }
}

void exit_with(var self) {
  With* iwith = Type_Class(type_of(self), With);
  if(iwith->exit) {
    iwith->exit(self);
  }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "+Prelude.h"

const char* type_name(Type t) {
  return type_class_name(t, "__TypeName");
}

static void type_print(Type t) {
  
  printf("(Type: '%p')\n", t);
  while(t->class_name) {
    if (strcmp(t->class_name, "__TypeName") == 0) {
      printf("(%s, %p '%s')\n", t->class_name, t->class_object, (const char*)t->class_object);
    } else {
      printf("(%s, %p)\n", t->class_name, t->class_object);
    }
    t++;
  }
  
}

bool type_implements_name(Type t, const char* class_name) {
  
  while(t->class_name) {
    if (strcmp(t->class_name, class_name) == 0) return true;
    t++;
  }
  
  return false;
}

var type_class_name(Type t, const char* class_name) {
  
  const char* type_name = NULL;
  
  while(t->class_name) {
    if (strcmp(t->class_name, "__TypeName") == 0) {
      type_name = t->class_object;
    }
    if (strcmp(t->class_name, class_name) == 0) {
      return (var)t->class_object;
    }
    t++;
  }
  
  if (type_name == NULL) {
    fprintf(stderr, "|\n| ObjectError: Cannot find type name for object.\n| Does is start with a 'Type' struct entry?\n|\n");
    abort();
  }
  
  fprintf(stderr, "|\n| ClassError: Type '%s' does not implement class '%s'\n|\n",  
    type_name, class_name);
  abort();
}

Type type_of(var self) {
  return ((ObjectObject*)self)->type;
}

var type_cast(var self, Type t, const char* func) {
  
  if (type_of(self) is t) {
    return self;
  } else {
    fprintf(stderr, "|\n| TypeError: Argument to function '%s'\n|\t Got type '%s'\n|\t Expected type '%s'\n|\n",
        func, type_name(type_of(self)), type_name(t)); abort();
  }
  
}

var new(Type type, ...) { 
  
  New* inew = type_class(type, New);
  
  var obj = calloc(inew->size, 1);
  ((ObjectObject*)obj)->type = type;
  
  if (inew->ctor) {
    va_list args;
    va_start(args, type);
    inew->ctor(obj, &args);
    va_end(args);
  }
  
  return obj;
}

void delete(var self) {
  New* inew = type_class(type_of(self), New);
  
  if (inew->dtor) {
    inew->dtor(self);
  }
  
  free(self);
}

var copy(var self) {
  Copy* icopy = type_class(type_of(self), Copy);
  assert(icopy->copy);
  return icopy->copy(self);
}

bool eq(var lhs, var rhs) {
  
  if (type_implements(type_of(lhs), Eq)) {
    Eq* ieq = type_class(type_of(lhs), Eq);
    assert(ieq->eq);
    return ieq->eq(lhs, rhs);
  } else {
    return lhs == rhs;
  }
  
}

bool neq(var lhs, var rhs) {
  return !eq(lhs, rhs);
}

bool gt(var lhs, var rhs) {
  Ord* iord = type_class(type_of(lhs), Ord);
  assert(iord->gt);
  return iord->gt(lhs, rhs);
}

bool lt(var lhs, var rhs) {
  Ord* iord = type_class(type_of(lhs), Ord);
  assert(iord->lt);
  return iord->lt(lhs, rhs);
}

bool ge(var lhs, var rhs) {
  return !lt(lhs, rhs);
}

bool le(var lhs, var rhs) {
  return !gt(lhs, rhs);
}

size_t len(var self) {
  Len* ilen = type_class(type_of(self), Len);
  assert(ilen->len);
  return ilen->len(self);
}

bool contains(var self, var obj) {
  Contains* icontains = type_class(type_of(self), Contains);
  assert(icontains->contains);
  return icontains->contains(self, obj);
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

bool is_empty(var self) {
  Empty* iempty = type_class(type_of(self), Empty);
  assert(iempty->is_empty);
  return iempty->is_empty(self);
}

void clear(var self) {
  Empty* iempty = type_class(type_of(self), Empty);
  assert(iempty->clear);
  iempty->clear(self);
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

int hash(var self) {
  Hash* ihash = type_class(type_of(self), Hash);
  assert(ihash->hash);
  return ihash->hash(self);
}

var get(var self, const char* key) {
  Dict* idict = type_class(type_of(self), Dict);
  assert(idict->get);
  return idict->get(self, key);
}

void put(var self, const char* key, var value) {
  Dict* idict = type_class(type_of(self), Dict);
  assert(idict->put);
  idict->put(self, key, value);
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





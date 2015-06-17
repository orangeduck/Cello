#include "Cello.h"

var _ = CelloEmpty(_);
var Terminal = CelloEmpty(Terminal);

static const char* Iter_Name(void) {
  return "Iter";
}

static const char* Iter_Brief(void) {
  return "Iterable";
}

static const char* Iter_Description(void) {
  return
    "The `Iter` class is implemented by types which can be looped over. This "
    "allows them to be used in conjunction with the `foreach` macro as well "
    "as various other components of Cello."
    "\n\n"
    "To signal that an interation has finished an iteration should return the "
    "Cello object `Terminal`. Due to this - the `Terminal` object cannot be "
    "placed inside of Tuples because it artificially shortens their length.";
}

static const char* Iter_Definition(void) {
  return
    "struct Iter {\n"
    "  var (*iter_init)(var);\n"
    "  var (*iter_next)(var, var);\n"
    "  var (*iter_prev)(var, var);\n"
    "  var (*iter_last)(var);\n"
    "  var (*iter_type)(var);\n"
    "};\n";
}

static struct Example* Iter_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = new(Array, Int, $I(1), $I(2), $I(5));\n"
      "\n"
      "foreach(o in x) {\n"
      "  show(o); /* 1, 2, 5 */\n"
      "}\n"
    }, {
      "Table",
      "var prices = new(Table, String, Int);\n"
      "set(prices, $S(\"Apple\"),  $I(12));\n"
      "set(prices, $S(\"Banana\"), $I( 6));\n"
      "set(prices, $S(\"Pear\"),   $I(55));\n"
      "\n"
      "foreach(key in prices) {\n"
      "  var price = get(prices, key);\n"
      "  print(\"Price of %$ is %$\\n\", key, price);\n"
      "}\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Iter_Methods(void) {
  
  static struct Method methods[] = {
    {
      "foreach", 
      "#define foreach(...)\n",
      "Iterate over elements in a loop."
    }, {
      "iter_init", 
      "var iter_init(var self);\n"
      "var iter_last(var self);",
      "Return the initial item (or final item) in the iteration over `self`."
    }, {
      "iter_next", 
      "var iter_next(var self, var curr);\n"
      "var iter_prev(var self, var curr);",
      "Given the current item `curr`, return the next (or previous) item in "
      "the iteration over `self`."
    }, {
      "iter_type", 
      "var iter_type(var self);",
      "Returns the type of item that can be expected to be returned by the "
      "iterable."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var Iter = Cello(Iter,
  Instance(Doc,
    Iter_Name,       Iter_Brief,    Iter_Description, 
    Iter_Definition, Iter_Examples, Iter_Methods));

var iter_init(var self) {
  return method(self, Iter, iter_init);
}

var iter_next(var self, var curr) {
  return method(self, Iter, iter_next, curr);
}

var iter_last(var self) {
  return method(self, Iter, iter_last);
}

var iter_prev(var self, var curr) {
  return method(self, Iter, iter_prev, curr);
}

var iter_type(var self) {
  return method(self, Iter, iter_type);  
}

static const char* Range_Name(void) {
  return "Range";
}

static const char* Range_Brief(void) {
  return "Integer Sequence";
}

static const char* Range_Description(void) {
  return
    "The `Range` type is a basic iterable which acts as a virtual "
    "sequence of integers, starting from some value, stopping at some value "
    "and incrementing by some step."
    "\n\n"
    "This can be a useful replacement for the standard C `for` loop with "
    "decent performance but returning a Cello `Int`. It is constructable on "
    "the stack with the `range` macro which makes it practical and easy to "
    "use.";
}

static const char* Range_Definition(void) {
  return
    "struct Range {\n"
    "  var value;\n"
    "  int64_t start;\n"
    "  int64_t stop;\n"
    "  int64_t step;\n"
    "};\n";
}

static struct Example* Range_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "/* Iterate 0 to 10 */\n"
      "foreach (i in range($I(10))) {\n"
      "  print(\"%i\\n\", i);\n"
      "}\n"
      "\n"
      "/* Iterate 10 to 20 */\n"
      "foreach (i in range($I(10), $I(20))) {\n"
      "  print(\"%i\\n\", i);\n"
      "}\n"
      "\n"
      "/* Iterate 10 to 20 with a step of 5 */\n"
      "foreach (i in range($I(10), $I(20), $I(5))) {\n"
      "  print(\"%i\\n\", i);\n"
      "}\n"
      "\n"
      "/* Iterate 20 to 10 */\n"
      "foreach (i in range($I(10), $I(20), $I(-1))) {\n"
      "  print(\"%i\\n\", i);\n"
      "}\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Range_Methods(void) {
  
  static struct Method methods[] = {
    {
      "range", 
      "#define range(...)",
      "Construct a `Range` object on the stack."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var range_stack(var self, var args) {
  
  struct Range* r = self;
  size_t nargs = len(args);
  
  if (nargs > 3) {
    throw(FormatError, "Received too many arguments to Range constructor");
  }
  
  switch (nargs) {
    case 0:
      r->start = 0;
      r->stop  = 0;
      r->step  = 1;
    break;
    case 1:
      r->start = 0;
      r->stop  = c_int(get(args, $I(0)));
      r->step  = 1;
    break;
    case 2:
      r->start = get(args, $I(0)) is _ ? 0 : c_int(get(args, $I(0)));
      r->stop  = c_int(get(args, $I(1)));
      r->step  = 1;
    break;
    case 3:
      r->start = get(args, $I(0)) is _ ? 0 : c_int(get(args, $I(0)));
      r->stop  = c_int(get(args, $I(1)));
      r->step  = get(args, $I(2)) is _ ? 1 : c_int(get(args, $I(2)));
    break;
  }
  
  return self;
  
}

static void Range_New(var self, var args) {
  struct Range* r = self;
  r->value = new(Int);  
  range_stack(self, args);
}

static void Range_Del(var self) {
  struct Range* r = self;
  del(r->value);
}

static void Range_Assign(var self, var obj) {
  struct Range* r = self;
  struct Range* o = cast(obj, Range);
  assign(r->value, o->value);
  r->start = o->start;
  r->stop = o->stop;
  r->step = o->step;
}

static int Range_Cmp(var self, var obj) {
  struct Range* r = self;
  struct Range* o = cast(obj, Range);
  return memcmp(&r->start, &o->start, sizeof(int64_t) * 3);
}

static var Range_Iter_Init(var self) {
  struct Range* r = self;
  struct Int* i = r->value;
  if (r->step == 0) { return Terminal; }
  if (r->step  > 0) { i->val = r->start; }
  if (r->step  < 0) { i->val = r->stop-1; }
  if (r->step  > 0 and i->val >= r->stop) { return Terminal; }
  if (r->step  < 0 and i->val < r->start) { return Terminal; }
  return i;
}

static var Range_Iter_Last(var self) {
  struct Range* r = self;
  struct Int* i = r->value;
  if (r->step == 0) { return Terminal; }
  if (r->step  > 0) { i->val = r->stop-1; }
  if (r->step  < 0) { i->val = r->start; }
  if (r->step  > 0 and i->val < r->start) { return Terminal; }
  if (r->step  < 0 and i->val >= r->stop) { return Terminal; }
  return i;
}

static var Range_Iter_Next(var self, var curr) {
  struct Range* r = self;
  struct Int* i = r->value;
  i->val += r->step;
  if (r->step == 0) { return Terminal; }
  if (r->step  > 0 and i->val >= r->stop) { return Terminal; }
  if (r->step  < 0 and i->val < r->start) { return Terminal; }
  return i;
}

static var Range_Iter_Prev(var self, var curr) {
  struct Range* r = self;
  struct Int* i = r->value;
  i->val -= r->step;
  if (r->step == 0) { return Terminal; }
  if (r->step  > 0 and i->val < r->start) { return Terminal; }
  if (r->step  < 0 and i->val >= r->stop) { return Terminal; }
  return i;
}

static var Range_Iter_Type(var self) {
  return Int;
}

static size_t Range_Len(var self) {
  struct Range* r = self;
  if (r->step == 0) { return 0; }
  if (r->step  > 0) { return ((r->stop-1) - r->start) /  r->step + 1; }
  if (r->step  < 0) { return ((r->stop-1) - r->start) / -r->step + 1; }
  return 0;
}

static var Range_Get(var self, var key) {
  struct Range* r = self;
  struct Int* x = r->value;
  
  int64_t i = c_int(key);
  i = i < 0 ? Range_Len(r)+i : i;
  
  if (r->step == 0) {
    x->val = 0;
    return x;
  }
  
  if (r->step  > 0 and (r->start + r->step * i) < r->stop) {
    x->val = r->start  + r->step * i;
    return x;
  }
  
  if (r->step  < 0 and (r->stop-1 + r->step * i) >= r->start) {
    x->val = r->stop-1 + r->step * i;
    return x;
  }
  
  return throw(IndexOutOfBoundsError, 
      "Index '%i' out of bounds for Range of start %i, stop %i and step %i.", 
      key, $I(r->start), $I(r->stop), $I(r->step));
}

static bool Range_Mem(var self, var key) {
  struct Range* r = self;
  int64_t i = c_int(key);
  i = i < 0 ? Range_Len(r)+i : i;
  if (r->step == 0) { return false; }
  if (r->step  > 0) {
    return i >= r->start and i < r->stop and (i - r->start) % r->step is 0;
  }
  if (r->step  < 0) {
    return i >= r->start and i < r->stop and (i - (r->stop-1)) % -r->step is 0;
  }
  return false;
}

static int Range_Show(var self, var output, int pos) {
  struct Range* r = self;
  pos = print_to(output, pos, "<'Range' At 0x%p [", self);
  var curr = Range_Iter_Init(self);
  while (curr isnt Terminal) {
    pos = print_to(output, pos, "%i", curr);
    curr = Range_Iter_Next(self, curr);
    if (curr isnt Terminal) { pos = print_to(output, pos, ", "); }
  }
  return print_to(output, pos, "]>");
}

var Range = Cello(Range,
  Instance(Doc,
    Range_Name,       Range_Brief,    Range_Description, 
    Range_Definition, Range_Examples, Range_Methods),
  Instance(New,       Range_New, Range_Del),
  Instance(Assign,    Range_Assign),
  Instance(Cmp,       Range_Cmp),
  Instance(Len,       Range_Len),
  Instance(Get,       Range_Get, NULL, Range_Mem, NULL),
  Instance(Show,      Range_Show, NULL),
  Instance(Iter, 
    Range_Iter_Init,  Range_Iter_Next, 
    Range_Iter_Last,  Range_Iter_Prev, Range_Iter_Type));

static const char* Slice_Name(void) {
  return "Slice";
}

static const char* Slice_Brief(void) {
  return "Partial Iterable";
}

static const char* Slice_Description(void) {
  return
    "The `Slice` type is an iterable that allows one to only iterate over "
    "part of another iterable. Given some start, stop and step, only "
    "those entries described by the `Slice` are returned in the iteration."
    "\n\n"
    "Under the hood the `Slice` object still iterates over the whole iterable "
    "but it only returns those values in the range given.";
}

static const char* Slice_Definition(void) {
  return
    "struct Slice {\n"
    "  var iter;\n"
    "  var range;\n"
    "};\n";
}

static struct Example* Slice_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var x = tuple(\n"
      "  $S(\"Hello\"), $S(\"There\"), $S(\"World\"), $S(\"!\"));\n"
      "\n"
      "/* Iterate over elements 0 to 2 */\n"
      "foreach (s in slice(x, $I(2))) {\n"
      "  print(\"%s\\n\", s);\n"
      "}\n"
      "\n"
      "/* Iterate over elements 1 to 2 */\n"
      "foreach (s in slice(x, $I(1), $I(2))) {\n"
      "  print(\"%s\\n\", s);\n"
      "}\n"
      "\n"
      "/* Iterate over every other element */\n"
      "foreach (s in slice(x, _, _, $I(2))) {\n"
      "  print(\"%s\\n\", s);\n"
      "}\n"
      "\n"
      "/* Iterate backwards, starting from element 3 */\n"
      "foreach (s in slice(x, _, $I(2), $I(-1))) {\n"
      "  print(\"%s\\n\", s);\n"
      "}\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Slice_Methods(void) {
  
  static struct Method methods[] = {
    {
      "slice", 
      "#define slice(I, ...)",
      "Construct a `Slice` object on the stack over iterable `I`."
    }, {
      "reverse",
      "#define reverse(I)",
      "Construct a `Slice` object that iterates over iterable `I` in reverse "
      "order."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

static int64_t Slice_Arg(int part, size_t n, var arg) {
  
  if (arg is _) {
    if (part is 0) { return 0; }
    if (part is 1) { return n; }
    if (part is 2) { return 1; }
  }
  
  int64_t a = c_int(arg);
  
  if (part isnt 2) {
    a = a < 0 ? n+a : a;
    a = a > n ? n   : a;
    a = a < 0 ? 0   : a;
  }
  
  return a;
}

var slice_stack(var self, var args) {
  
  size_t nargs = len(args);

  if (nargs > 4) {
    throw(FormatError, "Received too many arguments to Slice constructor");
  }
  
  if (nargs < 1) {
    throw(FormatError, "Received too few arguments to Slice constructor");
  }
  
  struct Slice* s = self;
  s->iter  = get(args, $I(0));
  
  struct Range* r = s->range;
  size_t n = len(s->iter);
  
  switch (nargs) {
    case 1:
      r->start = 0;
      r->stop  = n;
      r->step  = 1;
    break;
    case 2:
      r->start = 0;
      r->stop  = Slice_Arg(1, n, get(args, $I(1)));
      r->step  = 1;
    break;
    case 3:
      r->start = Slice_Arg(0, n, get(args, $I(1)));
      r->stop  = Slice_Arg(1, n, get(args, $I(2)));
      r->step  = 1;
    break;
    case 4:
      r->start = Slice_Arg(0, n, get(args, $I(1)));
      r->stop  = Slice_Arg(1, n, get(args, $I(2)));
      r->step  = Slice_Arg(2, n, get(args, $I(3)));
    break;
  }
  
  return self;
  
}

static void Slice_New(var self, var args) {
  struct Slice* s = self;
  s->range = new(Range);
  slice_stack(self, args);
}

static void Slice_Del(var self) {
  struct Slice* s = self;
  del(s->range);
}

static void Slice_Assign(var self, var obj) {
  struct Slice* s = self;
  struct Slice* o = cast(obj, Slice);
  s->iter = o->iter;
  assign(s->range, o->range);
}

static int Slice_Cmp(var self, var obj) {
  struct Slice* s = self;
  struct Slice* o = cast(obj, Slice);
  if (s->iter > o->iter) { return  1; }
  if (s->iter < o->iter) { return -1; }
  return cmp(s->range, o->range);
}

static var Slice_Iter_Init(var self) {
  struct Slice* s = self;
  struct Range* r = s->range;
  
  if (r->step > 0) {
    var curr = iter_init(s->iter);
    for(int64_t i = 0; i < r->start; i++) {
      curr = iter_next(s->iter, curr);
    }
    return curr;
  }
  
  if (r->step < 0) {
    var curr = iter_last(s->iter);
    for (int64_t i = 0; i < (int64_t)len(s->iter)-r->stop; i++) {
      curr = iter_prev(s->iter, curr);
    }
    return curr;
  }

  return Terminal;
}

static var Slice_Iter_Next(var self, var curr) {
  struct Slice* s = self;
  struct Range* r = s->range;
  
  if (r->step > 0) {
    for (int64_t i = 0; i < r->step; i++) {
      curr = iter_next(s->iter, curr);
    }
  }
  
  if (r->step < 0) {
    for (int64_t i = 0; i < -r->step; i++) {
      curr = iter_prev(s->iter, curr);
    }
  }
  
  return curr;
}

static var Slice_Iter_Type(var self) {
  struct Slice* s = self;
  return iter_type(s->iter);
}

static var Slice_Iter_Last(var self) {
  struct Slice* s = self;
  struct Range* r = s->range;
  
  if (r->step > 0) {
    var curr = iter_last(s->iter);
    for(int64_t i = 0; i < (int64_t)len(s->iter)-r->stop; i++) {
      curr = iter_prev(s->iter, curr);
    }
    return curr;
  }
  
  if (r->step < 0) {
    var curr = iter_init(s->iter);
    for(int64_t i = 0; i < r->start; i++) {
      curr = iter_next(s->iter, curr);
    }
    return curr;
  }

  return Terminal;
}

static var Slice_Iter_Prev(var self, var curr) {
  struct Slice* s = self;
  struct Range* r = s->range;
  
  if (r->step > 0) {
    for (int64_t i = 0; i < r->step; i++) {
      curr = iter_prev(s->iter, curr);
    }
  }
  
  if (r->step < 0) {
    for (int64_t i = 0; i < -r->step; i++) {
      curr = iter_next(s->iter, curr);
    }
  }
  
  return curr;
}

static size_t Slice_Len(var self) {
  struct Slice* s = self;
  return Range_Len(s->range);
}

static var Slice_Get(var self, var key) {
  struct Slice* s = self;
  return get(s->iter, Range_Get(s->range, key));
}

static bool Slice_Mem(var self, var key) {
  var curr = Slice_Iter_Init(self);
  while (curr) {
    if (eq(curr, key)) { return true; }
    curr = Slice_Iter_Next(self, curr);
  }
  return false;
}

static int Slice_Show(var self, var output, int pos) {
  struct Slice* s = self;
  pos = print_to(output, pos, "<'Slice' At 0x%p [", self);
  var curr = Slice_Iter_Init(self);
  while (curr isnt Terminal) {
    pos = print_to(output, pos, "%$", curr);
    curr = Slice_Iter_Next(self, curr);
    if (curr isnt Terminal) { pos = print_to(output, pos, ", "); }
  }
  return print_to(output, pos, "]>");
}

var Slice = Cello(Slice,
  Instance(Doc,
    Slice_Name,       Slice_Brief,    Slice_Description, 
    Slice_Definition, Slice_Examples, Slice_Methods),
  Instance(New,      Slice_New, Slice_Del),
  Instance(Assign,   Slice_Assign),
  Instance(Cmp,      Slice_Cmp),
  Instance(Len,      Slice_Len),
  Instance(Get,      Slice_Get, NULL, Slice_Mem, NULL),
  Instance(Iter, 
    Slice_Iter_Init, Slice_Iter_Next, 
    Slice_Iter_Last, Slice_Iter_Prev, Slice_Iter_Type),
  Instance(Show,     Slice_Show, NULL));
  
  
static const char* Zip_Name(void) {
  return "Zip";
}

static const char* Zip_Brief(void) {
  return "Multiple Iterator";
}

static const char* Zip_Description(void) {
  return
    "The `Zip` type can be used to combine multiple iterables into one which "
    "is then iterated over all at once and returned as a Tuple. The Zip object "
    "only iterates when all of it's sub iterators have valid items. More "
    "specifically the Zip iteration will terminate if _any_ of the sub "
    "iterators terminate.";
}

static const char* Zip_Definition(void) {
  return
    "struct Zip {\n"
    "  var iters;\n"
    "  var values;\n"
    "};\n";
}

static struct Example* Zip_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "/* Iterate over two iterables at once */\n"
      "var x = new(Array, Int, $I(100), $I(200), $I(130));\n"
      "var y = new(Array, Float, $F(0.1), $F(0.2), $F(1.3));\n"
      "foreach (pair in zip(x, y)) {\n"
      "  print(\"x: %$\\n\", get(pair, $I(0)));\n"
      "  print(\"y: %$\\n\", get(pair, $I(1)));\n"
      "}\n"
      "\n"
      "/* Iterate over iterable with count */\n"
      "foreach (pair in enumerate(x)) {\n"
      "  print(\"%i: %$\\n\", get(pair, $I(0)), get(pair, $I(1)));\n"
      "}\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Zip_Methods(void) {
  
  static struct Method methods[] = {
    {
      "zip", 
      "#define zip(...)",
      "Construct a `Zip` object on the stack."
    }, {
      "enumerate", 
      "#define enumerate(I)",
      "Zip the iterable `I` with a `Range` object of the same length."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

var zip_stack(var self) {
  struct Zip* z = self;
  size_t nargs = len(z->iters);
  struct Tuple* t = z->values;
  for (size_t i = 0; i < nargs; i++) {
    t->items[i] = _;
  }
  t->items[nargs] = Terminal;
  return z;
}

static void Zip_New(var self, var args) {
  struct Zip* z = self;
  z->iters = new(Tuple);
  z->values = new(Tuple);
  assign(z->iters, args);
  for (size_t i = 0; i < len(args); i++) {
    push(z->values, _);
  }
}

static void Zip_Del(var self) {
  struct Zip* z = self;
  del(z->iters);
  del(z->values);
}

static void Zip_Assign(var self, var obj) {
  struct Zip* z = self;
  struct Zip* o = cast(obj, Zip);
  assign(z->iters, o->iters);
  assign(z->values, o->values);
}

static var Zip_Iter_Init(var self) {
  struct Zip* z = self;
  struct Tuple* values = z->values;
  struct Tuple* iters = z->iters;
  size_t num = len(iters);
  if (num is 0) { return Terminal; }
  for (size_t i = 0; i < num; i++) {
    var init = iter_init(iters->items[i]);
    if (init is Terminal) { return Terminal; }
    values->items[i] = init;
  }
  return values;
}

static var Zip_Iter_Last(var self) {
  struct Zip* z = self;
  struct Tuple* values = z->values;
  struct Tuple* iters = z->iters;
  size_t num = len(iters);
  if (num is 0) { return Terminal; }
  for (size_t i = 0; i < num; i++) {
    var last = iter_last(iters->items[i]);
    if (last is Terminal) { return Terminal; }
    values->items[i] = last;
  }
  return values;
}

static var Zip_Iter_Next(var self, var curr) {
  struct Zip* z = self;
  struct Tuple* values = z->values;
  struct Tuple* iters = z->iters;
  size_t num = len(iters);
  if (num is 0) { return Terminal; }
  for (size_t i = 0; i < num; i++) {
    var next = iter_next(iters->items[i], get(curr, $I(i)));
    if (next is Terminal) { return Terminal; }
    values->items[i] = next;
  }
  return values;
}

static var Zip_Iter_Prev(var self, var curr) {
  struct Zip* z = self;
  struct Tuple* values = z->values;
  struct Tuple* iters = z->iters;
  size_t num = len(iters);
  if (num is 0) { return Terminal; }
  for (size_t i = 0; i < num; i++) {
    var prev = iter_prev(iters->items[i], get(curr, $I(i)));
    if (prev is Terminal) { return Terminal; }
    values->items[i] = prev;
  }
  return values;
}

static var Zip_Iter_Type(var self) {
  return Tuple;
}

static size_t Zip_Len(var self) {
  struct Zip* z = self;
  struct Tuple* values = z->values;
  struct Tuple* iters = z->iters;
  size_t num = len(iters);
  if (num is 0) { return 0; }
  size_t mlen = len(iters->items[0]);
  for (size_t i = 1; i < num; i++) {
    size_t num = len(iters->items[i]);
    mlen = num < mlen ? num : mlen;
  }
  return mlen;
}

static var Zip_Get(var self, var key) {
  struct Zip* z = self;
  struct Tuple* values = z->values;
  struct Tuple* iters = z->iters;
  size_t num = len(iters);
  
  for (size_t i = 0; i < num; i++) {
    values->items[i] = get(iters->items[i], key);
  }
  
  return values;
}

static bool Zip_Mem(var self, var key) {
  foreach (item in self) {
    if (eq(item, key)) { return true; }
  }
  return false;
}

var Zip = Cello(Zip,
  Instance(Doc,
    Zip_Name,         Zip_Brief,    Zip_Description, 
    Zip_Definition,   Zip_Examples, Zip_Methods),
  Instance(New,       Zip_New, Zip_Del),
  Instance(Assign,    Zip_Assign),
  Instance(Len,       Zip_Len),
  Instance(Get,       Zip_Get, NULL, Zip_Mem, NULL),
  Instance(Iter, 
    Zip_Iter_Init,  Zip_Iter_Next, 
    Zip_Iter_Last,  Zip_Iter_Prev, Zip_Iter_Type));

var enumerate_stack(var self) {
  struct Zip* z = self;
  struct Range* r = get(z->iters, $I(0));
  r->stop = len(get(z->iters, $I(1)));
  return self;
}

static const char* Filter_Name(void) {
  return "Filter";
}

static const char* Filter_Brief(void) {
  return "Filtered Iterable";
}

static const char* Filter_Description(void) {
  return
    "The `Filter` type can be used to filter the results of some iterable. "
    "Given a callable object `Filter` iterable returns only those items in "
    "the original iterable for where calling the function returns a "
    "non-`NULL` value.";
}

static const char* Filter_Definition(void) {
  return
    "struct Filter {\n"
    "  var iter;\n"
    "  var func;\n"
    "};\n";
}

static struct Example* Filter_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var greater_than_two(var x) {\n"
      "  return c_int(x) > 2 ? x : NULL;\n"
      "}\n"
      "\n"
      "var x = new(Array, Int, $I(0), $I(5), $I(2), $I(9));\n"
      "\n"
      "foreach (n in filter(x, $(Function, greater_than_two))) {\n"
      "  show(n); /* 5, 9 */\n"
      "}\n"
    }, {
      "Usage 2",
      "var mem_hello(var x) {\n"
      "  return mem(x, $S(\"Hello\")) ? x : NULL;\n"
      "}\n"
      "\n"
      "var x = new(Tuple, \n"
      "  $S(\"Hello World\"), $S(\"Hello Dan\"), \n"
      "  $S(\"Bonjour\"));\n"
      "\n"
      "var y = new(Tuple);\n"
      "assign(y, filter(x, $(Function, mem_hello)));\n"
      "show(y); /* tuple(\"Hello World\", \"Hello Dan\") */\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static struct Method* Filter_Methods(void) {
  
  static struct Method methods[] = {
    {
      "filter", 
      "#define filter(I, F)",
      "Construct a `Filter` object on the stack over iterable `I` with "
      "filter function `F`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

static void Filter_New(var self, var args) {
  struct Filter* f = self;
  f->iter = get(args, $I(0));
  f->func = get(args, $I(1));
}

static var Filter_Iter_Init(var self) {
  struct Filter* f = self;
  var curr = iter_init(f->iter);
  while (true) {
    if (curr is Terminal or call_with(f->func, curr)) {
      return curr;
    } else {
      curr = iter_next(f->iter, curr);
    }
  }
  return Terminal;
}

static var Filter_Iter_Last(var self) {
  struct Filter* f = self;
  var curr = iter_last(f->iter);
  while (true) {
    if (curr is Terminal or call_with(f->func, curr)) {
      return curr;
    } else {
      curr = iter_prev(f->iter, curr);
    }
  }
  return Terminal;
}

static var Filter_Iter_Next(var self, var curr) {
  struct Filter* f = self;
  curr = iter_next(f->iter, curr);
  while (true) {
    if (curr is Terminal or call_with(f->func, curr)) {
      return curr;
    } else {
      curr = iter_next(f->iter, curr);
    }
  }
  return Terminal;
}

static var Filter_Iter_Prev(var self, var curr) {
  struct Filter* f = self;
  curr = iter_prev(f->iter, curr);
  while (true) {
    if (curr is Terminal or call_with(f->func, curr)) {
      return curr;
    } else {
      curr = iter_prev(f->iter, curr);
    }
  }
  return Terminal;
}

static var Filter_Iter_Type(var self) {
  struct Filter* f = self;
  return iter_type(f->iter);
}

static bool Filter_Mem(var self, var key) {
  foreach (item in self) {
    if (eq(item, key)) { return true; }
  }
  return false;
}

var Filter = Cello(Filter,
  Instance(Doc,
    Filter_Name,       Filter_Brief,    Filter_Description, 
    Filter_Definition, Filter_Examples, Filter_Methods),
  Instance(New,        Filter_New, NULL),
  Instance(Get,        NULL, NULL, Filter_Mem, NULL),
  Instance(Iter, 
    Filter_Iter_Init, Filter_Iter_Next, 
    Filter_Iter_Last, Filter_Iter_Prev, Filter_Iter_Type));
    
    
static const char* Map_Name(void) {
  return "Map";
}

static const char* Map_Brief(void) {
  return "Apply Function to Iterable";
}

static const char* Map_Description(void) {
  return
    "The `Map` type is an iterable that applies some callable to to each "
    "item in another iterable and returns the result. This can be useful to "
    "make more concise iteration when there are callback functions available."
    "\n\n"
    "If the mapping callable is a purely side-effect callable it is possible "
    "to use the `call` function on the `Map` object directly for a quick way "
    "to perform the iteration."
    "\n\n"
    "One downside of `Map` is that the `iter_type` becomes unknown (there is "
    "no way to know what type the callable will return so some objects such "
    "as `Array`s may revert to using `Ref` as the object type when assigned a "
    "`Map`.";
}

static const char* Map_Definition(void) {
  return
    "struct Map {\n"
    "  var iter;\n"
    "  var curr;\n"
    "  var func;\n"
    "};\n";
}

static struct Method* Map_Methods(void) {
  
  static struct Method methods[] = {
    {
      "map", 
      "#define map(I, F)",
      "Construct a `Map` object on the stack over iterable `I` applying "
      "function `F`."
    }, {NULL, NULL, NULL}
  };
  
  return methods;
}

static struct Example* Map_Examples(void) {
  
  static struct Example examples[] = {
    {
      "Usage",
      "var convert_to_int(var x) {\n"
      "  var y = new(Int);\n"
      "  look_from(y, x, 0);\n"
      "  return y;\n"
      "}\n"
      "\n"
      "var x = tuple($S(\"1\"), $S(\"2\"), $S(\"3\"));\n"
      "\n"
      "foreach (y in map(x, $(Function, convert_to_int))) {\n"
      "  show(y); /* 1, 2, 3 */\n"
      "};\n"
    }, {
      "Usage 2",
      "var print_object(var x) {\n"
      "  println(\"Object %$ is of type %$\", x, type_of(x));\n"
      "  return NULL;\n"
      "}\n"
      "\n"
      "var x = tuple($I(0), $S(\"Hello!\"), $F(2.4));\n"
      "\n"
      "call(map(x, $(Function, print_object)));\n"
    }, {NULL, NULL}
  };

  return examples;
  
}

static void Map_New(var self, var args) {
  struct Map* m = self;
  m->iter = get(args, $I(0));
  m->func = get(args, $I(1));
}

static var Map_Iter_Init(var self) {
  struct Map* m = self;
  m->curr = iter_init(m->iter);
  if (m->curr is Terminal) {
    return m->curr;
  } else {
    return call_with(m->func, m->curr);
  }
}

static var Map_Iter_Last(var self) {
  struct Map* m = self;
  m->curr = iter_last(m->iter);
  if (m->curr is Terminal) {
    return m->curr;
  } else {
    return call_with(m->func, m->curr);
  }
}

static var Map_Iter_Next(var self, var curr) {
  struct Map* m = self;
  m->curr = iter_next(m->iter, m->curr);
  if (m->curr is Terminal) {
    return m->curr;
  } else {
    return call_with(m->func, m->curr);
  }
}

static var Map_Iter_Prev(var self, var curr) {
  struct Map* m = self;
  m->curr = iter_prev(m->iter, m->curr);
  if (m->curr is Terminal) {
    return m->curr;
  } else {
    return call_with(m->func, m->curr);
  }
}

static size_t Map_Len(var self) {
  struct Map* m = self;
  return len(m->iter);
}

static var Map_Get(var self, var key) {
  struct Map* m = self;
  m->curr = get(m->iter, key);
  if (m->curr is Terminal) {
    return m->curr;
  } else {
    return call_with(m->func, m->curr);
  }
}

static bool Map_Mem(var self, var key) {
  foreach (item in self) {
    if (eq(item, key)) { return true; }
  }
  return false;
}

static var Map_Call(var self, var args) {
  foreach (item in self);
  return Terminal;
}

var Map = Cello(Map,
  Instance(Doc,
    Map_Name,       Map_Brief,    Map_Description, 
    Map_Definition, Map_Examples, Map_Methods),
  Instance(New,     Map_New, NULL),
  Instance(Len,     Map_Len),
  Instance(Get,     Map_Get, NULL, Map_Mem, NULL),
  Instance(Call,    Map_Call),
  Instance(Iter, 
    Map_Iter_Init, Map_Iter_Next, 
    Map_Iter_Last, Map_Iter_Prev, NULL));

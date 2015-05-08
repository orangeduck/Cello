#include "Cello.h"

var _ = CelloEmpty(Missing);
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
    "as various other components of Cello.";
}

static const char* Iter_Definition(void) {
  return
    "struct Iter {\n"
    "  var (*iter_init)(var);\n"
    "  var (*iter_next)(var, var);\n"
    "  var (*iter_prev)(var, var);\n"
    "  var (*iter_last)(var);\n"
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

void Range_New(var self, var args) {
  struct Range* r = self;
  size_t nargs = len(args);
  
  if (nargs > 4) {
    throw(FormatError, "Received too many arguments to Range constructor");
  }
  
  if (nargs < 1) {
    throw(FormatError, "Received too few arguments to Range constructor");
  }
  
  r->value = get(args, $I(0));
  
  switch (nargs) {
    case 2:
      r->start = 0;
      r->stop  = c_int(get(args, $I(1)));
      r->step  = 1;
    break;
    case 3:
      r->start = get(args, $I(1)) is _ ? 0 : c_int(get(args, $I(1)));
      r->stop  = c_int(get(args, $I(2)));
      r->step  = 1;
    break;
    case 4:
      r->start = get(args, $I(1)) is _ ? 0 : c_int(get(args, $I(1)));
      r->stop  = c_int(get(args, $I(2)));
      r->step  = get(args, $I(3)) is _ ? 1 : c_int(get(args, $I(3)));
    break;
  }
  
}

static var Range_Iter_Init(var self) {
  struct Range* r = self;
  struct Int* i = r->value;
  if (r->step == 0) { return NULL; }
  if (r->step  > 0) { i->val = r->start; }
  if (r->step  < 0) { i->val = r->stop-1; }
  if (r->step  > 0 and i->val >= r->stop) { return NULL; }
  if (r->step  < 0 and i->val < r->start) { return NULL; }
  return i;
}

static var Range_Iter_Last(var self) {
  struct Range* r = self;
  struct Int* i = r->value;
  if (r->step == 0) { return NULL; }
  if (r->step  > 0) { i->val = r->stop-1; }
  if (r->step  < 0) { i->val = r->start; }
  if (r->step  > 0 and i->val < r->start) { return NULL; }
  if (r->step  < 0 and i->val >= r->stop) { return NULL; }
  return i;
}

static var Range_Iter_Next(var self, var curr) {
  struct Range* r = self;
  struct Int* i = r->value;
  i->val += r->step;
  if (r->step == 0) { return NULL; }
  if (r->step  > 0 and i->val >= r->stop) { return NULL; }
  if (r->step  < 0 and i->val < r->start) { return NULL; }
  return i;
}

static var Range_Iter_Prev(var self, var curr) {
  struct Range* r = self;
  struct Int* i = r->value;
  i->val -= r->step;
  if (r->step == 0) { return NULL; }
  if (r->step  > 0 and i->val < r->start) { return NULL; }
  if (r->step  < 0 and i->val >= r->stop) { return NULL; }
  return i;
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

static var Range_Subtype(var self) {
  return Int;
}

static int Range_Show(var self, var output, int pos) {
  struct Range* r = self;
  pos = print_to(output, pos, "<'Range' At 0x%p [", self);
  var curr = Range_Iter_Init(self);
  while (curr) {
    pos = print_to(output, pos, "%i", curr);
    curr = Range_Iter_Next(self, curr);
    if (curr) { pos = print_to(output, pos, ", "); }
  }
  return print_to(output, pos, "]>");
}

var Range = Cello(Range,
  Instance(Doc,
    Range_Name,       Range_Brief,    Range_Description, 
    Range_Definition, Range_Examples, Range_Methods),
  Instance(New,       Range_New, NULL),
  Instance(Len,       Range_Len),
  Instance(Get,       Range_Get, NULL, Range_Mem, NULL),
  Instance(Subtype,   Range_Subtype),
  Instance(Show,      Range_Show, NULL),
  Instance(Iter, 
    Range_Iter_Init,  Range_Iter_Next, 
    Range_Iter_Last,  Range_Iter_Prev));

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

void Slice_New(var self, var args) {
  struct Slice* s = self;
  size_t nargs = len(args);
  
  if (nargs > 5) {
    throw(FormatError, "Received too many arguments to Slice constructor");
  }
  
  if (nargs < 2) {
    throw(FormatError, "Received few many arguments to Slice constructor");
  }
  
  s->iter  = get(args, $I(0));
  s->range = get(args, $I(1));
  
  struct Range* r = s->range;
  size_t n = len(s->iter);
  
  switch (nargs) {
    case 2:
      r->start = 0;
      r->stop  = n;
      r->step  = 1;
    break;
    case 3:
      r->start = 0;
      r->stop  = Slice_Arg(1, n, get(args, $I(2)));
      r->step  = 1;
    break;
    case 4:
      r->start = Slice_Arg(0, n, get(args, $I(2)));
      r->stop  = Slice_Arg(1, n, get(args, $I(3)));
      r->step  = 1;
    break;
    case 5:
      r->start = Slice_Arg(0, n, get(args, $I(2)));
      r->stop  = Slice_Arg(1, n, get(args, $I(3)));
      r->step  = Slice_Arg(2, n, get(args, $I(4)));
    break;
  }
  
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
    for(int64_t i = 0; i < (int64_t)len(s->iter)-r->stop; i++) {
      curr = iter_prev(s->iter, curr);
    }
    return curr;
  }

  return NULL;
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

/* TODO: Test */
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

  return NULL;
}

/* TODO: Test */
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

static var Slice_Subtype(var self) {
  struct Slice* s = self;
  return subtype(s->iter);
}

static int Slice_Show(var self, var output, int pos) {
  struct Slice* s = self;
  pos = print_to(output, pos, "<'Slice' At 0x%p [", self);
  var curr = Slice_Iter_Init(self);
  while (curr) {
    pos = print_to(output, pos, "%$", curr);
    curr = Slice_Iter_Next(self, curr);
    if (curr) { pos = print_to(output, pos, ", "); }
  }
  return print_to(output, pos, "]>");
}

var Slice = Cello(Slice,
  Instance(Doc,
    Slice_Name,       Slice_Brief,    Slice_Description, 
    Slice_Definition, Slice_Examples, Slice_Methods),
  Instance(New,      Slice_New, NULL),
  Instance(Subtype,  Slice_Subtype),
  Instance(Len,      Slice_Len),
  Instance(Get,      Slice_Get, NULL, Slice_Mem, NULL),
  Instance(Iter, 
    Slice_Iter_Init, Slice_Iter_Next, 
    Slice_Iter_Last, Slice_Iter_Prev),
  Instance(Show,     Slice_Show, NULL));




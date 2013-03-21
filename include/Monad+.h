#ifndef MonadPlus_h
#define MonadPlus_h

class {
  var (*mbind)(var, var);
  var (*mthen)(var, var);
  var (*mreturn)(var);
} Monad;

data {
  var type;
  var val;
} Maybe;

var Maybe_Just(var a);
var Maybe_None();
var Maybe_Bind(var ma, var mb);
var Maybe_Then(var ma, var mb);

instance(Maybe, Monad) = { Maybe_Bind, Maybe_Then, Maybe_Just };

data {
  var type;
  var val;
} IO;

var IO_Return(var a);
var IO_Bind(var ma, var mb);
var IO_Then(var ma, var mb);

instance(IO, Monad) = { IO_Bind, IO_Then, IO_Return };

#endif

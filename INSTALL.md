Install
=======


Compiling
---------

One can build the source distribution via:

    $ make

This will build `libCello.a` and `libCello.so`


Installing
----------

One can install headers and libraries via:

    $ make install

This will attempt to copy `libCello.so` to the local libraries installation path.
It will also copy the headers into the local headers path.


Running the tests
-----------------

One can run the tests as follows:

    $ make check

Making the demos
----------------

One can make all the demos as follows:

    $ make demos
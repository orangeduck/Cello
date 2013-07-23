Install
=======

Installing Cello requires a GNU99 compatible compiler. On Linux and Mac `gcc` is the recommended choice while on windows the _MinGW64_ version of `gcc` is recommended.

For any issues please contact me at: `contact@theorangeduck.com`

Compiling
---------

One can build the source distribution using

    $ make

This will build `libCello.a` and `libCello.so` which can then be included directly into any projects as required.


Installing
----------

One can attempt install headers and libraries to the system locations using

    $ make install

This will attempt to copy `libCello.so`, `libCello.a` and the headers to the system local locations.

If this does so incorrectly users are encouraged to move the libraries and headers by hand.


Running the tests
-----------------

One can run the tests using

    $ make check
    

Making the demos
----------------

One can make all the demos using

    $ make demos
    
    

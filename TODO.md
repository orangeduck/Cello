Cello 2.0
=========

* Garbage Collection
  - Heap Objects: All heap objects are roots.
  - Stack Objects: All stack objects are roots.
    We can treat stack object pointers the same as GC list pointers. Actual
    pointer address returned by `alloc_stk` is recorded.
    
  - GC Objects: When allocating store address of allocation location on stack as
    well as allocated pointer address. If this address gets changed (or sp goes 
    below allocation address) we know the variable has been mangled and that it 
    can be removed from the root list.

* Fix Map Type
* Unit/Empty Type for empty container?
* Alignment/padding on Array and Table
* Optimise type info lookup with Perfect hashing
* Type Assign/Copy class

* Cast Class
* Union Type Constructor
* Type Parenting?
* Type Forwarding?
* Update examples
* Update tests
* Update README
* Update INSTALL
* Update Syntax def for Notepad++
* Update Syntax def for gedit
* Update Documentation

----------------

* REPL
  - Python prototype
  - Parse CPP and extract defines.
  - Parse Syntax tree and extract type/variable declarations
  - Record those in perminant storage which is added to every file

Cello 2.0
=========

* Typeclass for key/val type of containers
* Update assigns for containers to look at types
* Quickcheck
* Help function

* Cast Class
* Update examples
* Update tests
  - Add test for perfect hashing
  - Add test for array alignment
* Update README
* Update INSTALL
* Update Syntax def for Notepad++
* Update Syntax def for gedit
* Update Documentation

----------------

* Garbage Collection
  - Heap Objects: All heap objects are roots.
  - Stack Objects: All stack objects are roots.
    We can treat stack object pointers the same as GC list pointers. Actual
    pointer address returned by `alloc_stk` is recorded.
    
  - GC Objects: When allocating store address of allocation location on stack as
    well as allocated pointer address. If this address gets changed (or sp goes 
    below allocation address) we know the variable has been mangled and that it 
    can be removed from the root list.


----------------

* REPL
  - Python prototype
  - Parse CPP and extract defines.
  - Parse Syntax tree and extract type/variable declarations
  - Record those in perminant storage which is added to every file

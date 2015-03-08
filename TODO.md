Cello 2.0
=========

* Fix Crashing Exceptions on Windows
* Remove all static variables
* Make garbage collection run based on allocation size not number of items
* Make garbage collection thread local

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

* REPL
  - Python prototype
  - Parse CPP and extract defines.
  - Parse Syntax tree and extract type/variable declarations
  - Record those in perminant storage which is added to every file

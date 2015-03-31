Cello 2.0
=========

Libraries (for now):
  * JSON
  * Quickcheck
  * Random
  * Markdown
  
* Make allocation not new register with gc and add "root" and "raw" variations
* Change Traverse Class to something GC specific
* Update documentation to say how GC is thread local
* Send/Recv class
* CELLO_NSTRACE flag
* CELLO_NGC flag
* memset with 0xDeadCe110
* Make GC disable/enableable

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

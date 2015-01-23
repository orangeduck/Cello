Cello 2.0
=========

* Generator/Range/Sequence class
* Just put instances in type object declaration
* Wrap libuv in Cello like layer
* Garbage collection
	- cleanup extension gcc/clang
* Disable features not available such as threads/exceptions/functions/gc
* REPL
	- Don't want to do anything by hand. Just interface strings.
	- Switch modes between top level and execution
	- `nm` tool to list symbols for autocomplete
* Unit Type for empty type
* Append Concat class
* Ref and Box Eq and Hash


* JIT compilation of methods. Keep most recently called methods in cache?
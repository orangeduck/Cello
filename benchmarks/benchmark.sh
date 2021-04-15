#!/bin/sh
gcc ./ext/genint.c -o ./ext/genint

gcc Nbodies/nbodies_c.c -std=c99 -O3 -lm -o Nbodies/nbodies_c
g++ Nbodies/nbodies_cpp.cpp -std=c++11 -O3 -lm -o Nbodies/nbodies_cpp
cc Nbodies/nbodies_cello.c -DCELLO_NDEBUG ../libCello.a -I../include -std=gnu99  -O3 -lm -lpthread -o Nbodies/nbodies_cello
javac Nbodies/nbodies_java.java

gcc List/list_c.c -Wno-unused-result -I./ext -std=c99 -O3 -lm -o List/list_c
g++ List/list_cpp.cpp -Wno-unused-result -std=c++11 -O3 -lm -o List/list_cpp
cc List/list_cello.c -DCELLO_NDEBUG ../libCello.a -I../include -Wno-unused-result -std=gnu99  -O3 -lm -lpthread -o List/list_cello
javac List/list_java.java

gcc Dict/dict_c.c -Wno-unused-result -I./ext -std=c99 -O3 -lm -o Dict/dict_c
g++ Dict/dict_cpp.cpp -Wno-unused-result -std=c++11 -O3 -lm -o Dict/dict_cpp
cc Dict/dict_cello.c -DCELLO_NDEBUG ../libCello.a -I../include -Wno-unused-result -std=gnu99  -O3 -lm -lpthread -o Dict/dict_cello
javac Dict/dict_java.java

gcc Map/map_c.c -Wno-unused-result -I./ext -std=c99 -O3 -lm -o Map/map_c
g++ Map/map_cpp.cpp -Wno-unused-result -std=c++11 -O3 -lm -o Map/map_cpp
cc Map/map_cello.c -DCELLO_NDEBUG ../libCello.a -I../include -Wno-unused-result -std=gnu99  -O3 -lm -lpthread -o Map/map_cello
javac Map/map_java.java

gcc Sudoku/sudoku_c.c -Wno-unused-result -I./ext -std=c99 -O3 -lm -o Sudoku/sudoku_c
g++ Sudoku/sudoku_cpp.cpp -Wno-unused-result -std=c++11 -O3 -lm -o Sudoku/sudoku_cpp
cc Sudoku/sudoku_cello.c -DCELLO_NDEBUG ../libCello.a -I../include -Wno-unused-result -std=gnu99  -O3 -lm -lpthread -o Sudoku/sudoku_cello
javac Sudoku/sudoku_java.java

gcc Matmul/matmul_c.c -Wno-unused-result -I./ext -std=c99 -O3 -lm -o Matmul/matmul_c
g++ Matmul/matmul_cpp.cpp -Wno-unused-result -std=c++11 -O3 -lm -o Matmul/matmul_cpp
cc Matmul/matmul_cello.c -DCELLO_NDEBUG ../libCello.a -I../include -Wno-unused-result -std=gnu99  -O3 -lm -lpthread -o Matmul/matmul_cello
javac Matmul/matmul_java.java

gcc GC/gc_c.c -Wno-unused-result -I./ext -std=c99 -O3 -lm -o GC/gc_c
g++ GC/gc_cpp.cpp -Wno-unused-result -std=c++11 -O3 -lm -o GC/gc_cpp
cc GC/gc_cello.c -DCELLO_NDEBUG ../libCello.a -I../include -std=gnu99 -O3 -lm -lpthread -o GC/gc_cello
javac GC/gc_java.java

echo 
echo "## Garbage Collection"
echo
printf "* C: "
gtime -f "%e" -f "%e" ./GC/gc_c
printf "* C++: "
gtime -f "%e" -f "%e" ./GC/gc_cpp
printf "* Cello: "
gtime -f "%e" -f "%e" ./GC/gc_cello
printf "* Java: "
gtime -f "%e" -f "%e" java -cp ./GC gc_java
printf "* Javascript: "
gtime -f "%e" -f "%e" node GC/gc_javascript.js
printf "* Python: "
gtime -f "%e" -f "%e" python GC/gc_python.py
printf "* Ruby: "
gtime -f "%e" -f "%e" ruby GC/gc_ruby.rb
printf "* Lua: "
gtime -f "%e" -f "%e" lua GC/gc_lua.lua
# printf "* Lua JIT: "
# gtime -f "%e" -f "%e" luajit GC/gc_lua.lua

# gprof GC/gc_cello > GC/profile.txt
# rm gmon.out


echo 
echo "## List"
echo
printf "* C: "
gtime -f "%e" -f "%e" ./List/list_c
printf "* C++: "
gtime -f "%e" -f "%e" ./List/list_cpp
printf "* Cello: "
gtime -f "%e" -f "%e" ./List/list_cello
printf "* Java: "
gtime -f "%e" -f "%e" java -cp ./List list_java
printf "* Javascript: "
gtime -f "%e" -f "%e" node List/list_javascript.js
printf "* Python: "
gtime -f "%e" -f "%e" python List/list_python.py
printf "* Ruby: "
gtime -f "%e" -f "%e" ruby List/list_ruby.rb
printf "* Lua: "
gtime -f "%e" -f "%e" lua List/list_lua.lua
# printf "* Lua JIT: "
# gtime -f "%e" -f "%e" luajit List/list_lua.lua

# gprof List/list_cello > List/profile.txt
# rm gmon.out

echo 
echo "## Map"
echo
printf "* C: "
gtime -f "%e" -f "%e" sh -c './ext/genint | ./Map/map_c'
printf "* C++: "
gtime -f "%e" -f "%e" sh -c './ext/genint | ./Map/map_cpp'
printf "* Cello: "
gtime -f "%e" -f "%e" sh -c './ext/genint | ./Map/map_cello'
printf "* Java: "
gtime -f "%e" -f "%e" sh -c './ext/genint | java -cp ./Map map_java'
printf "* Javascript: "
gtime -f "%e" -f "%e" sh -c './ext/genint | node Map/map_javascript.js'
printf "* Python: "
gtime -f "%e" -f "%e" sh -c './ext/genint | python Map/map_python.py'
printf "* Ruby: "
gtime -f "%e" -f "%e" sh -c './ext/genint | ruby Map/map_ruby.rb'
printf "* Lua: "
gtime -f "%e" -f "%e" sh -c './ext/genint | lua Map/map_lua.lua'
# printf "* Lua JIT: "
# gtime -f "%e" -f "%e" sh -c './ext/genint | luajit Map/map_lua.lua'

# gprof Map/map_cello > Map/profile.txt
# rm gmon.out

echo 
echo "## NBodies"
echo
printf "* C: "
gtime -f "%e" -f "%e" ./Nbodies/nbodies_c 
printf "* C++: "
gtime -f "%e" -f "%e" ./Nbodies/nbodies_cpp
printf "* Cello: "
gtime -f "%e" -f "%e" ./Nbodies/nbodies_cello
printf "* Java: "
gtime -f "%e" -f "%e" java -cp ./Nbodies nbodies_java
printf "* Javascript: "
gtime -f "%e" -f "%e" node Nbodies/nbodies_javascript.js
printf "* Python: "
gtime -f "%e" -f "%e" python Nbodies/nbodies_python.py
printf "* Ruby: "
gtime -f "%e" -f "%e" ruby Nbodies/nbodies_ruby.rb
printf "* Lua: "
gtime -f "%e" -f "%e" lua Nbodies/nbodies_lua.lua
# printf "* Lua JIT: "
# gtime -f "%e" -f "%e" luajit Nbodies/nbodies_lua.lua

# gprof Nbodies/nbodies_cello > Nbodies/profile.txt
# rm gmon.out

echo 
echo "## Dict"
echo
printf "* C: "
gtime -f "%e" -f "%e" sh -c './ext/genint | ./Dict/dict_c'
printf "* C++: "
gtime -f "%e" -f "%e" sh -c './ext/genint | ./Dict/dict_cpp'
printf "* Cello: "
gtime -f "%e" -f "%e" sh -c './ext/genint | ./Dict/dict_cello'
printf "* Java: "
gtime -f "%e" -f "%e" sh -c './ext/genint | java -cp ./Dict dict_java'
printf "* Javascript: "
gtime -f "%e" -f "%e" sh -c './ext/genint | node Dict/dict_javascript.js'
printf "* Python: "
gtime -f "%e" -f "%e" sh -c './ext/genint | python Dict/dict_python.py'
printf "* Ruby: "
gtime -f "%e" -f "%e" sh -c './ext/genint | ruby Dict/dict_ruby.rb'
printf "* Lua: "
gtime -f "%e" -f "%e" sh -c './ext/genint | lua Dict/dict_lua.lua'
# printf "* Lua JIT: "
# gtime -f "%e" -f "%e" sh -c './ext/genint | luajit Dict/dict_lua.lua'

# gprof Dict/dict_cello > Dict/profile.txt
# rm gmon.out

echo 
echo "## Sudoku"
echo
printf "* C: "
gtime -f "%e" -f "%e" sh -c './ext/sudoku | ./Sudoku/sudoku_c'
printf "* C++: "
gtime -f "%e" -f "%e" sh -c './ext/sudoku | ./Sudoku/sudoku_cpp'
printf "* Cello: "
gtime -f "%e" -f "%e" sh -c './ext/sudoku | ./Sudoku/sudoku_cello'
printf "* Java: "
gtime -f "%e" -f "%e" sh -c './ext/sudoku | java -cp ./Sudoku sudoku_java'
printf "* Javascript: "
gtime -f "%e" -f "%e" sh -c './ext/sudoku | node Sudoku/sudoku_javascript.js'
printf "* Python: "
gtime -f "%e" -f "%e" sh -c './ext/sudoku | python Sudoku/sudoku_python.py'
printf "* Ruby: "
gtime -f "%e" -f "%e" sh -c './ext/sudoku | ruby Sudoku/sudoku_ruby.rb'
printf "* Lua: "
gtime -f "%e" -f "%e" sh -c './ext/sudoku | lua Sudoku/sudoku_lua.lua'
# printf "* Lua JIT: "
# gtime -f "%e" -f "%e" sh -c './ext/sudoku | luajit Sudoku/sudoku_lua.lua'

# gprof Sudoku/sudoku_cello > Sudoku/profile.txt
# rm gmon.out

echo 
echo "## Matmul"
echo
printf "* C: "
gtime -f "%e" -f "%e" ./Matmul/matmul_c
printf "* C++: "
gtime -f "%e" -f "%e" ./Matmul/matmul_cpp
printf "* Cello: "
gtime -f "%e" -f "%e" ./Matmul/matmul_cello
printf "* Java: "
gtime -f "%e" -f "%e" java -cp ./Matmul matmul_java
printf "* Javascript: "
gtime -f "%e" -f "%e" node Matmul/matmul_javascript.js
printf "* Python: "
gtime -f "%e" -f "%e" python Matmul/matmul_python.py
printf "* Ruby: "
gtime -f "%e" -f "%e" ruby Matmul/matmul_ruby.rb
printf "* Lua: "
gtime -f "%e" -f "%e" lua Matmul/matmul_lua.lua
# printf "* Lua JIT: "
# gtime -f "%e" -f "%e" luajit Matmul/matmul_lua.lua

# gprof Matmul/matmul_cello > Matmul/profile.txt
# rm gmon.out


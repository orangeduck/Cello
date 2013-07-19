
#include "ptest.h"
#include "Cello.h"

#include <math.h>

PT_SUITE(suite_data) {

  PT_TEST(test_bool_ord) {
    PT_ASSERT(eq(True, True));
    PT_ASSERT(eq(False, False));
    PT_ASSERT(gt(True, False));
    PT_ASSERT(lt(False, True)); 
  }

  PT_TEST(test_bool_hash) {
    PT_ASSERT(hash(True) is 1);
    PT_ASSERT(hash(False) is 0);
  }

  PT_TEST(test_bool_asctypes) {
    PT_ASSERT(as_long(True) is 1);
    PT_ASSERT(as_long(False) is 0);
    PT_ASSERT(as_double(True) is 1.0);
    PT_ASSERT(as_double(False) is 0.0);
    PT_ASSERT_STR_EQ(as_str(False), "False");
    PT_ASSERT_STR_EQ(as_str(True), "True");
  }

  PT_TEST(test_char_create) {
    
    var char0 = $(Char, 'a');
    var char1 = new(Char, 'b');
    var char2 = copy(char0);
    
    PT_ASSERT(char0);
    PT_ASSERT(char1);
    
    PT_ASSERT(as_char(char0) is 'a');
    PT_ASSERT(as_char(char1) is 'b');
    PT_ASSERT(as_char(char2) is 'a');
    
    assign(char1, char2);
    
    PT_ASSERT(char1 isnt char2);
    PT_ASSERT(as_char(char1) is 'a');
    
    delete(char1);
    delete(char2);
    
  }

  PT_TEST(test_char_ord) {
    PT_ASSERT( gt($(Char, 'b'), $(Char, 'a')) );
    PT_ASSERT( gt($(Char, 'z'), $(Char, 'g')) );
    PT_ASSERT( gt($(Char, 'a'), $(Char, 'A')) );
    PT_ASSERT( gt($(Char, 'G'), $(Char, 'F')) );
    PT_ASSERT( lt($(Char, 'V'), $(Char, 't')) );
    PT_ASSERT( lt($(Char, 'C'), $(Char, 'x')) );
    PT_ASSERT( lt($(Char, 'Y'), $(Char, 'Z')) );
    PT_ASSERT( eq($(Char, 'h'), $(Char, 'h')) );
    PT_ASSERT( ge($(Char, 'T'), $(Char, 'T')) );
    PT_ASSERT( le($(Char, 'A'), $(Char, 'a')) );
    PT_ASSERT( le($(Char, 'a'), $(Char, 'a')) );
    PT_ASSERT( neq($(Char, 'T'), $(Char, 'V')) );
  }

  PT_TEST(test_char_hash) {
    PT_ASSERT( hash($(Char, 'A')) is 65);
    PT_ASSERT( hash($(Char, 'b')) is 98);
    PT_ASSERT( hash($(Char, 'z')) is 122);
  }

  PT_TEST(test_int_create) {
    
    var int0 = $(Int, 1);
    var int1 = new(Int, 24313);
    var int2 = copy(int0);
    
    PT_ASSERT(int0);
    PT_ASSERT(int1);
    PT_ASSERT(int2);
    
    PT_ASSERT(as_long(int0) is 1);
    PT_ASSERT(as_long(int1) is 24313);
    PT_ASSERT(as_long(int2) is 1);
    
    assign(int2, int1);
    
    PT_ASSERT( not(int2 is int1) );
    PT_ASSERT(as_long(int2) is 24313);
    
    delete(int1);
    delete(int2);
    
  }

  PT_TEST(test_int_ord) {
    
    PT_ASSERT( gt($(Int, 5),    $(Int, 0)) );
    PT_ASSERT( gt($(Int, 5133), $(Int, 232)) );
    PT_ASSERT( lt($(Int, 2332), $(Int, 9992)) );
    PT_ASSERT( lt($(Int, 34),   $(Int, 323)) );
    PT_ASSERT( eq($(Int, 111),  $(Int, 111)) );
    PT_ASSERT( eq($(Int, 23),   $(Int, 23)) );
    PT_ASSERT( ge($(Int, 289),  $(Int, 289)) );
    PT_ASSERT( ge($(Int, 8734), $(Int, 289)) );
    PT_ASSERT( le($(Int, 166),  $(Int, 166)) );
    PT_ASSERT( le($(Int, 11),   $(Int, 888)) );
    PT_ASSERT( neq($(Int, 324), $(Int, 685)) );
    PT_ASSERT( neq($(Int, 34),  $(Int, 54)) );
    
  }

  PT_TEST(test_int_hash) {
    
    PT_ASSERT( hash($(Int, 34)) is 34 );
    PT_ASSERT( hash($(Int, 11)) is 11 );
    PT_ASSERT( hash($(Int, 06)) is 06 );
    PT_ASSERT( hash($(Int, 8213)) is 8213 );
    
  }

  PT_TEST(test_int_num) {
    
    var tester = $(Int, 0);
    
    PT_ASSERT(as_long(tester) is 0);
    
    add(tester, $(Int, 10)); PT_ASSERT(as_long(tester) is 10);
    sub(tester, $(Int, 4)); PT_ASSERT(as_long(tester) is 6);
    mul(tester, $(Int, 10)); PT_ASSERT(as_long(tester) is 60);
    divide(tester, $(Int, 3)); PT_ASSERT(as_long(tester) is 20);
    negate(tester); PT_ASSERT(as_long(tester) is -20);
    absolute(tester); PT_ASSERT(as_long(tester) is 20);
    absolute(tester); PT_ASSERT(as_long(tester) is 20);
    
  }

  PT_TEST(test_int_parse) {
    
    var f = open($(File, NULL), "test.bin", "w");
    
    put(f, Int, $(Int, 10));
    put(f, Int, $(Int, 32));
    
    close(f);
    
    f = open($(File, NULL), "test.bin", "r");
    
    var fst = get(f, Int);
    var snd = get(f, Int);
    
    close(f);
    
    PT_ASSERT(as_long(fst) is 10);
    PT_ASSERT(as_long(snd) is 32);
    
    delete(fst);
    delete(snd);
    
  }

  PT_TEST(test_real_create) {
    
    var real0 = $(Real, 1.0);
    var real1 = new(Real, 24.313);
    var real2 = copy(real0);
    
    PT_ASSERT(real0);
    PT_ASSERT(real1);
    PT_ASSERT(real2);
    
    PT_ASSERT(as_double(real0) is 1.0);
    PT_ASSERT(as_double(real1) is 24.313);
    PT_ASSERT(as_double(real2) is 1.0);
    
    assign(real2, real1);
    
    PT_ASSERT( not(real2 is real1) );
    PT_ASSERT(as_double(real2) is 24.313);
    
    delete(real1);
    delete(real2);
    
  }

  PT_TEST(test_real_ord) {
    
    PT_ASSERT( gt($(Real, 5.0),    $(Real, 0.0)) );
    PT_ASSERT( gt($(Real, 51.33), $(Real, 2.32)) );
    PT_ASSERT( lt($(Real, 23.32), $(Real, 99.92)) );
    PT_ASSERT( lt($(Real, 31.0),   $(Real, 32.3)) );
    PT_ASSERT( eq($(Real, 1.11),  $(Real, 1.11)) );
    PT_ASSERT( eq($(Real, 23.55),   $(Real, 23.55)) );
    PT_ASSERT( ge($(Real, 2.89),  $(Real, 2.89)) );
    PT_ASSERT( ge($(Real, 87.34), $(Real, 2.89)) );
    PT_ASSERT( le($(Real, 16.6),  $(Real, 16.6)) );
    PT_ASSERT( le($(Real, 1.1),   $(Real, 88.8)) );
    PT_ASSERT( neq($(Real, 3.24), $(Real, 6.85)) );
    PT_ASSERT( neq($(Real, 3.4),  $(Real, 5.4)) );
    
  }

  union type_interp {
    double as_double;
    long as_long;
  };

  PT_TEST(test_real_hash) {
    
    union type_interp r0 = { 34.0 };
    union type_interp r1 = { 11.0 };
    union type_interp r2 = { 0.6 };
    union type_interp r3 = { 82.13 };
    
    PT_ASSERT( hash($(Real, 34.0)) is r0.as_long );
    PT_ASSERT( hash($(Real, 11.0)) is r1.as_long );
    PT_ASSERT( hash($(Real, 0.6)) is r2.as_long );
    PT_ASSERT( hash($(Real, 82.13)) is r3.as_long );
    
  }

  PT_TEST(test_real_num) {
    
    double base = 0.0;
    var mirror = $(Real, 0.0);
    
    base += 0.1; add(mirror, $(Real, 0.1));
    PT_ASSERT(as_double(mirror) is base);
    
    base *= 4.0; mul(mirror, $(Real, 4.0));
    PT_ASSERT(as_double(mirror) is base);
    
    base /= 2.0; divide(mirror, $(Real, 2.0));
    PT_ASSERT(as_double(mirror) is base);
    
    base = -base; negate(mirror);
    PT_ASSERT(as_double(mirror) is base);  
    
    base = fabs(base); absolute(mirror);
    PT_ASSERT(as_double(mirror) is base);  
    
  }

  PT_TEST(test_real_parse) {
    
    var f = open($(File, NULL), "test.bin", "w");
    
    put(f, Real, $(Real, 1.0));
    put(f, Real, $(Real, 3.2));
    
    close(f);
    
    f = open($(File, NULL), "test.bin", "r");
    
    var fst = get(f, Real);
    var snd = get(f, Real);
    
    close(f);
    
    PT_ASSERT(as_double(fst) is 1.0);
    PT_ASSERT(as_double(snd) is 3.2);
    
    delete(fst);
    delete(snd);
    
  }

  PT_TEST(test_string_create) {
    
    var s0 = $(String, "Hello");
    var s1 = new(String, "There");
    var s2 = copy(s0);
    
    PT_ASSERT(s0);
    PT_ASSERT(s1);
    PT_ASSERT(s2);
    
    PT_ASSERT_STR_EQ(as_str(s0), "Hello");
    PT_ASSERT_STR_EQ(as_str(s1), "There");
    PT_ASSERT_STR_EQ(as_str(s2), "Hello");
    
    assign(s2, s1);
    
    PT_ASSERT( not(s1 is s2) );
    PT_ASSERT_STR_EQ( as_str(s2), "There" );
    
    delete(s1);
    delete(s2);
    
  }

  PT_TEST(test_string_ord) {
    
    PT_ASSERT( gt($(String, "Apple"),    $(String, "Ball")) );
    PT_ASSERT( gt($(String, "cat"), $(String, "dog")) );
    PT_ASSERT( lt($(String, "great"), $(String, "hello")) );
    PT_ASSERT( lt($(String, "Yellow"),   $(String, "Zoo")) );
    PT_ASSERT( eq($(String, "Hello"),  $(String, "Hello")) );
    PT_ASSERT( eq($(String, "there"),   $(String, "there")) );
    PT_ASSERT( ge($(String, "Hello"),  $(String, "Hello")) );
    PT_ASSERT( ge($(String, "tooting"), $(String, "Red")) );
    PT_ASSERT( le($(String, "guard"),  $(String, "guardian")) );
    PT_ASSERT( le($(String, "keep"),   $(String, "keep")) );
    PT_ASSERT( neq($(String, "Hello"), $(String, "hello")) );
    PT_ASSERT( neq($(String, "group"),  $(String, "GROUP")) );
    
  }

  PT_TEST(test_string_collection) {
    
    var s0 = new(String, "Balloons");
    
    PT_ASSERT(len(s0) is 8);
    PT_ASSERT(contains(s0, $(String, "Ball")));
    PT_ASSERT(contains(s0, $(String, "oon")));
    PT_ASSERT(contains(s0, $(String, "Balloons")));
    PT_ASSERT(contains(s0, $(Char, 'l')));
    
    discard(s0, $(String, "oons"));
    
    PT_ASSERT_STR_EQ(as_str(s0), "Ball");
    
    clear(s0);
    
    PT_ASSERT(len(s0) is 0);
    PT_ASSERT_STR_EQ(as_str(s0), "");
    
    
    delete(s0);
    
  }

  PT_TEST(test_string_hash) {
    
    PT_ASSERT(hash($(String, "Hello")) is 511);
    PT_ASSERT(hash($(String, "There")) is 515);
    PT_ASSERT(hash($(String, "People")) is 629);
    
  }

  PT_TEST(test_string_reverse) {
    
    var s0 = new(String, "Hello");
    var s1 = new(String, "olleH");
    
    PT_ASSERT( neq(s0, s1) );
    
    reverse(s0);
    
    PT_ASSERT( eq(s0, s1) );
    
    delete(s0);
    delete(s1);
    
  }

  PT_TEST(test_array_create) {
    
    var a0 = new(Array, Int, 3, $(Int, 1), $(Int, 5), $(Int, 10));
    var a1 = new(Array, Real, 2, $(Real, 1.1), $(Real, 2.2));
    var a2 = copy(a0);
    
    PT_ASSERT(a0);
    PT_ASSERT(a1);
    PT_ASSERT(a2);
    
    PT_ASSERT(a0 isnt a1);
    PT_ASSERT(a0 isnt a2);
    PT_ASSERT(a1 isnt a2);
    
    PT_ASSERT( eq(at(a0, 0), $(Int, 1)) );
    PT_ASSERT( eq(at(a1, 0), $(Real, 1.1)) );
    PT_ASSERT( eq(at(a2, 0), $(Int, 1)) );
    
    assign(a2, a1);
    
    PT_ASSERT(a2 isnt a1);
    PT_ASSERT( eq(at(a2, 0), $(Real, 1.1)) );
    
    delete(a0);
    delete(a1);
    delete(a2);
    
  }

  PT_TEST(test_array_eq) {
    
    var a0 = new(Array, Int, 3, $(Int, 1), $(Int, 5), $(Int, 10));
    var a1 = new(Array, Int, 3, $(Int, 1), $(Int, 5), $(Int, 10));
    var a2 = new(Array, Int, 3, $(Int, 2), $(Int, 5), $(Int, 10));
    
    PT_ASSERT(a0 isnt a1);
    PT_ASSERT(a0 isnt a2);
    
    PT_ASSERT( eq(a0, a1) );
    PT_ASSERT( neq(a0, a2) );
    
    delete(a0);
    delete(a1);
    delete(a2);
    
  }

  PT_TEST(test_array_collection) {
    
    var a0 = new(Array, Int, 3, $(Int, 1), $(Int, 5), $(Int, 10));
    
    PT_ASSERT(len(a0) is 3);
    PT_ASSERT(contains(a0, $(Int, 1)));
    PT_ASSERT(contains(a0, $(Int, 5)));
    
    discard(a0, $(Int, 5));
    
    PT_ASSERT(len(a0) is 2);
    PT_ASSERT(contains(a0, $(Int, 1)));
    PT_ASSERT(not contains(a0, $(Int, 5)));
    
    clear(a0);
    
    PT_ASSERT(len(a0) is 0);
    PT_ASSERT(not contains(a0, $(Int, 1)));
    
    delete(a0);
    
  }

  PT_TEST(test_array_push) {
    
    var a0 = new(Array, Int, 0);
    
    PT_ASSERT(len(a0) is 0);
    
    push(a0, $(Int, 1));
    
    PT_ASSERT(len(a0) is 1);
    PT_ASSERT( eq(at(a0, 0), $(Int, 1)) );
    
    push_back(a0, $(Int, 3));
    
    PT_ASSERT(len(a0) is 2);
    PT_ASSERT( eq(at(a0, 0), $(Int, 1)) );
    PT_ASSERT( eq(at(a0, 1), $(Int, 3)) );
    
    push_front(a0, $(Int, 10));
    
    PT_ASSERT(len(a0) is 3);
    PT_ASSERT( eq(at(a0, 0), $(Int, 10)) );
    PT_ASSERT( eq(at(a0, 1), $(Int, 1)) );
    PT_ASSERT( eq(at(a0, 2), $(Int, 3)) );
    
    push_at(a0, $(Int, 20), 1);
    
    PT_ASSERT(len(a0) is 4);
    PT_ASSERT( eq(at(a0, 0), $(Int, 10)) );
    PT_ASSERT( eq(at(a0, 1), $(Int, 20)) );
    PT_ASSERT( eq(at(a0, 2), $(Int, 1)) );
    PT_ASSERT( eq(at(a0, 3), $(Int, 3)) );
    
    pop_at(a0, 2);
    
    PT_ASSERT(len(a0) is 3);
    PT_ASSERT( eq(at(a0, 0), $(Int, 10)) );
    PT_ASSERT( eq(at(a0, 1), $(Int, 20)) );
    PT_ASSERT( eq(at(a0, 2), $(Int, 3)) );
    
    pop_front(a0);
    
    PT_ASSERT(len(a0) is 2);
    PT_ASSERT( eq(at(a0, 0), $(Int, 20)) );
    PT_ASSERT( eq(at(a0, 1), $(Int, 3)) );
    
    pop_back(a0);
    
    PT_ASSERT(len(a0) is 1);
    PT_ASSERT( eq(at(a0, 0), $(Int, 20)) );
    
    pop(a0);
    
    PT_ASSERT(len(a0) is 0);
    
    delete(a0);
    
  }

  PT_TEST(test_array_at) {
    
    var a0 = new(Array, String, 3, $(String, "Hello"), $(String, "There"), $(String, "People"));
    
    PT_ASSERT( eq(at(a0, 0), $(String, "Hello")) );
    PT_ASSERT( eq(at(a0, 1), $(String, "There")) );
    PT_ASSERT( eq(at(a0, 2), $(String, "People")) );
    
    set(a0, 1, $(String, "Blah"));
    
    PT_ASSERT( eq(at(a0, 0), $(String, "Hello")) );
    PT_ASSERT( eq(at(a0, 1), $(String, "Blah")) );
    PT_ASSERT( eq(at(a0, 2), $(String, "People")) );
    
    set(a0, 0, $(String, "Foo"));
    set(a0, 2, $(String, "Bar"));
    
    PT_ASSERT( eq(at(a0, 0), $(String, "Foo")) );
    PT_ASSERT( eq(at(a0, 1), $(String, "Blah")) );
    PT_ASSERT( eq(at(a0, 2), $(String, "Bar")) );
    
    delete(a0);
    
  }

  PT_TEST(test_array_iter) {
    
    var a0 = new(Array, String, 3, $(String, "Hello"), $(String, "There"), $(String, "People"));
    
    int counter = 0;
    
    foreach(item in a0) {
      
      switch(counter) {
        case 0: PT_ASSERT( eq(item, $(String, "Hello")) ); break;
        case 1: PT_ASSERT( eq(item, $(String, "There")) ); break;
        case 2: PT_ASSERT( eq(item, $(String, "People")) ); break;
      }
      
      counter++;
    }
    
    PT_ASSERT(counter is 3);
    
    counter = 0;
    
    foreach(item0 in a0) {
      foreach(item1 in a0) {
        counter++;
      }
    }
    
    PT_ASSERT(counter is 9);
    
  }

  PT_TEST(test_array_reverse) {
    
    var a0 = new(Array, String, 3, $(String, "Hello"), $(String, "There"), $(String, "People"));
    var a1 = new(Array, String, 3, $(String, "People"), $(String, "There"), $(String, "Hello"));
    
    PT_ASSERT( not eq(a0, a1) );
    
    reverse(a0);
    
    PT_ASSERT( eq(a0, a1) );
    
  }

  PT_TEST(test_table_create) {
    
    var t0 = new(Table, String, Int);
    put(t0, $(String, "Hello"), $(Int, 2));
    put(t0, $(String, "There"), $(Int, 5));
    
    var t1 = new(Table, String, Int);
    put(t1, $(String, "Bonjour"), $(Int, 9));
    put(t1, $(String, "Where"), $(Int, 5));
    
    var t2 = copy(t0);
    
    PT_ASSERT(t0);
    PT_ASSERT(t1);
    PT_ASSERT(t2);
    
    PT_ASSERT(t0 isnt t2);
    PT_ASSERT(t0 isnt t1);
    
    PT_ASSERT(contains(t0, $(String, "Hello")));
    PT_ASSERT(contains(t1, $(String, "Bonjour")));
    PT_ASSERT(contains(t2, $(String, "There")));
    
    assign(t2, t1);
    
    PT_ASSERT(contains(t2, $(String, "Where")));
    PT_ASSERT(contains(t2, $(String, "Bonjour")));
    PT_ASSERT(not contains(t2, $(String, "Hello")));
    PT_ASSERT(not contains(t2, $(String, "There")));
    
    delete(t0);
    delete(t1);
    delete(t2);
    
  }

  PT_TEST(test_table_collection) {
    
    var t0 = new(Table, String, Int);
    put(t0, $(String, "Hello"), $(Int, 2));
    put(t0, $(String, "There"), $(Int, 5));
    
    PT_ASSERT(len(t0) is 2);
    PT_ASSERT(contains(t0, $(String, "Hello")));
    
    discard(t0, $(String, "Hello"));
    
    PT_ASSERT(len(t0) is 1);
    PT_ASSERT(not contains(t0, $(String, "Hello")));
    PT_ASSERT(contains(t0, $(String, "There")));
    
    clear(t0);
    
    PT_ASSERT(len(t0) is 0);
    PT_ASSERT(not contains(t0, $(String, "Hello")));
    PT_ASSERT(not contains(t0, $(String, "There")));
    
    delete(t0);
    
  }

  PT_TEST(test_table_dict) {
    
    var t0 = new(Table, String, Int);
    put(t0, $(String, "Hello"), $(Int, 2));
    put(t0, $(String, "There"), $(Int, 5));
    
    var i0 = get(t0, $(String, "Hello"));
    var i1 = get(t0, $(String, "There"));
    
    PT_ASSERT( eq(i0, $(Int, 2)) );
    PT_ASSERT( eq(i1, $(Int, 5)) );
    
    put(t0, $(String, "Hello"), $(Int, 6));
    
    var i2 = get(t0, $(String, "Hello"));
    PT_ASSERT( eq(i2, $(Int, 6)) );
    
    delete(t0);
    
  }

  PT_TEST(test_table_iter) {
    
    var t0 = new(Table, String, Int);
    put(t0, $(String, "Hello"), $(Int, 2));
    put(t0, $(String, "There"), $(Int, 5));
    
    int counter = 0;
    
    foreach(key in t0) {
      
      var val = get(t0, key);
      
      switch(counter) {
        case 0: PT_ASSERT( eq(key, $(String, "Hello")) ); break;
        case 1: PT_ASSERT( eq(key, $(String, "There")) ); break;
      }
      
      switch(counter) {
        case 0: PT_ASSERT( eq(val, $(Int, 2)) ); break;
        case 1: PT_ASSERT( eq(val, $(Int, 5)) ); break;
      }
      
      counter++;
    }
    
    PT_ASSERT(counter is 2);
    
  }

  PT_TEST(test_tree_create) {

    var t0 = new(Tree, String, Int);
    put(t0, $(String, "Hello"), $(Int, 2));
    put(t0, $(String, "There"), $(Int, 5));
    
    var t1 = new(Tree, String, Int);
    put(t1, $(String, "Bonjour"), $(Int, 9));
    put(t1, $(String, "Where"), $(Int, 5));
    
    var t2 = copy(t0);
    
    PT_ASSERT(t0);
    PT_ASSERT(t1);
    PT_ASSERT(t2);
    
    PT_ASSERT(t0 isnt t2);
    PT_ASSERT(t0 isnt t1);
    
    PT_ASSERT(contains(t0, $(String, "Hello")));
    PT_ASSERT(contains(t1, $(String, "Bonjour")));
    PT_ASSERT(contains(t2, $(String, "There")));
    
    assign(t2, t1);
    
    PT_ASSERT(contains(t2, $(String, "Where")));
    PT_ASSERT(contains(t2, $(String, "Bonjour")));
    PT_ASSERT(not contains(t2, $(String, "Hello")));
    PT_ASSERT(not contains(t2, $(String, "There")));
    
    delete(t0);
    delete(t1);
    delete(t2);

  }

  PT_TEST(test_tree_collection) {

    var t0 = new(Tree, String, Int);
    put(t0, $(String, "Hello"), $(Int, 2));
    put(t0, $(String, "There"), $(Int, 5));
    
    PT_ASSERT(len(t0) is 2);
    PT_ASSERT(contains(t0, $(String, "Hello")));
    
    discard(t0, $(String, "Hello"));
    
    PT_ASSERT(len(t0) is 1);
    PT_ASSERT(not contains(t0, $(String, "Hello")));
    PT_ASSERT(contains(t0, $(String, "There")));
    
    clear(t0);
    
    PT_ASSERT(len(t0) is 0);
    PT_ASSERT(not contains(t0, $(String, "Hello")));
    PT_ASSERT(not contains(t0, $(String, "There")));
    
    delete(t0);

  }

  PT_TEST(test_tree_dict) {

    var t0 = new(Tree, String, Int);
    put(t0, $(String, "Hello"), $(Int, 2));
    put(t0, $(String, "There"), $(Int, 5));
    
    var i0 = get(t0, $(String, "Hello"));
    var i1 = get(t0, $(String, "There"));
    
    PT_ASSERT( eq(i0, $(Int, 2)) );
    PT_ASSERT( eq(i1, $(Int, 5)) );

    put(t0, $(String, "There"), $(Int, 8));

    var i2 = get(t0, $(String, "There"));
    PT_ASSERT( eq(i2, $(Int, 8)) );   
    
    delete(t0);

  }

  PT_TEST(test_tree_iter) {

    var t0 = new(Tree, String, Int);
    put(t0, $(String, "Hello"), $(Int, 2));
    put(t0, $(String, "There"), $(Int, 5));
    
    int counter = 0;
    
    foreach(key in t0) {
      
      var val = get(t0, key);
      
      switch(counter) {
        case 0: PT_ASSERT( eq(key, $(String, "Hello")) ); break;
        case 1: PT_ASSERT( eq(key, $(String, "There")) ); break;
      }
      
      switch(counter) {
        case 0: PT_ASSERT( eq(val, $(Int, 2)) ); break;
        case 1: PT_ASSERT( eq(val, $(Int, 5)) ); break;
      }
      
      counter++;
    }
    
    PT_ASSERT(counter is 2);

  }

  #define TEST_VALUES() \
  var li0 = $(Int, 0); \
  var li1 = $(Real, 5.0); \
  var li2 = $(Int, 7); \
  var li3 = $(String, "Hello"); \
  var li4 = $(Real, 0.0); \
  var li5 = $(String, "There"); \
  var li6 = $(String, "There"); \
  var mk0 = $(String, "Hello"); \
  var mk1 = $(String, "There"); \
  var mk2 = $(String, "Blah"); \
  var mk3 = $(String, "Looking"); \
  var mk4 = $(String, "Balloons"); \
  var mk5 = $(String, "Gravy"); \
  var dk0 = $(String, "Hello"); \
  var dk1 = $(String, "There"); \
  var dk2 = $(Int, 25); \
  var dk3 = $(Int, 4); \
  var dk4 = $(Real, 7.7); \
  var dk5 = $(String, "Gravy"); \
  var dv0 = $(Int, 5); \
  var dv1 = $(Int, 231); \
  var dv2 = $(Int, 543); \
  var dv3 = $(Int, 9592); \
  var dv4 = $(Int, 933); \
  var dv5 = $(Real, 5.8); \
  var dv6 = $(Real, 23.1); \
  var dv7 = $(Real, 5.43); \
  var dv8 = $(Real, 959.211); \
  var dv9 = $(Real, 9.3321);

  PT_TEST(test_list_create) {
    
    TEST_VALUES();
    
    var l0 = new(List, 4, li0, li1, li2, li3);
    var l1 = new(List, 2, li4, li5);
    var l2 = copy(l0);
    
    PT_ASSERT(l0);
    PT_ASSERT(l1);
    PT_ASSERT(l2);
    
    PT_ASSERT(l0 isnt l1);
    PT_ASSERT(l0 isnt l2);
    PT_ASSERT(l1 isnt l2);
    
    PT_ASSERT(at(l0, 0) is li0);
    PT_ASSERT(at(l0, 2) is li2);
    PT_ASSERT(at(l1, 0) is li4);
    PT_ASSERT(at(l1, 1) is li5);
    PT_ASSERT(at(l2, 0) is li0);
    PT_ASSERT(at(l2, 1) is li1);
    
    assign(l2, l1);
    
    PT_ASSERT(at(l2, 0) is li4);
    PT_ASSERT(at(l2, 1) is li5);
    
    delete(l0);
    delete(l1);
    delete(l2);
    
  }

  PT_TEST(test_list_eq) {
    
    TEST_VALUES();
    
    var l0 = new(List, 4, li0, li1, li2, li3);
    var l1 = new(List, 2, li4, li5);
    var l2 = new(List, 4, li0, li1, li2, li3);
    var l3 = new(List, 2, li0, li6);
    
    PT_ASSERT(l0 isnt l1);
    PT_ASSERT(l0 isnt l2);
    PT_ASSERT(l1 isnt l2);
    
    PT_ASSERT( eq(l0, l2) );
    PT_ASSERT( eq(l1, l3) );
    PT_ASSERT( neq(l0, l1) );
    PT_ASSERT( neq(l1, l2) );
    
    delete(l0);
    delete(l1);
    delete(l2);
    
  }

  PT_TEST(test_list_collection) {
    
    TEST_VALUES();
    
    var l0 = new(List, 4, li0, li1, li2, li3);
    
    PT_ASSERT(len(l0) is 4);
    PT_ASSERT(contains(l0, li0));
    PT_ASSERT(contains(l0, li1));
    PT_ASSERT(contains(l0, li2));
    PT_ASSERT(contains(l0, li3));
    PT_ASSERT(contains(l0, li4));
    PT_ASSERT(not contains(l0, li5));
    
    discard(l0, li0);
    
    PT_ASSERT(len(l0) is 3);
    PT_ASSERT(not contains(l0, li0));
    PT_ASSERT(contains(l0, li1));
    PT_ASSERT(contains(l0, li2));
    PT_ASSERT(contains(l0, li3));
    PT_ASSERT(not contains(l0, li4));
    PT_ASSERT(not contains(l0, li5));
    
    clear(l0);
    
    PT_ASSERT(len(l0) is 0);
    PT_ASSERT(not contains(l0, li0));
    PT_ASSERT(not contains(l0, li1));
    PT_ASSERT(not contains(l0, li2));
    PT_ASSERT(not contains(l0, li3));
    PT_ASSERT(not contains(l0, li4));
    PT_ASSERT(not contains(l0, li5));
    
    delete(l0);
    
  }

  PT_TEST(test_list_push) {
    
    TEST_VALUES();
    
    var l0 = new(List, 0);
    
    PT_ASSERT(len(l0) is 0);
    
    push(l0, li0);
    
    PT_ASSERT(len(l0) is 1);
    PT_ASSERT(at(l0, 0) is li0);
    
    push_back(l0, li1);

    PT_ASSERT(len(l0) is 2);
    PT_ASSERT(at(l0, 0) is li0);
    PT_ASSERT(at(l0, 1) is li1);
    
    push_front(l0, li2);
    
    PT_ASSERT(len(l0) is 3);
    PT_ASSERT(at(l0, 0) is li2);
    PT_ASSERT(at(l0, 1) is li0);
    PT_ASSERT(at(l0, 2) is li1);
    
    push_at(l0, li3, 1);
    
    PT_ASSERT(len(l0) is 4);
    PT_ASSERT(at(l0, 0) is li2);
    PT_ASSERT(at(l0, 1) is li3);
    PT_ASSERT(at(l0, 2) is li0);
    PT_ASSERT(at(l0, 3) is li1);
    
    pop_at(l0, 2);
    
    PT_ASSERT(len(l0) is 3);
    PT_ASSERT(at(l0, 0) is li2);
    PT_ASSERT(at(l0, 1) is li3);
    PT_ASSERT(at(l0, 2) is li1);
    
    pop_front(l0);
    
    PT_ASSERT(len(l0) is 2);
    PT_ASSERT(at(l0, 0) is li3);
    PT_ASSERT(at(l0, 1) is li1);
    
    pop_back(l0);
    
    PT_ASSERT(len(l0) is 1);
    PT_ASSERT(at(l0, 0) is li3);
    
    pop(l0);
    
    PT_ASSERT(len(l0) is 0);
    
    delete(l0);
    
  }

  PT_TEST(test_list_at) {
    
    TEST_VALUES();
    
    var l0 = new(List, 4, li0, li1, li2, li3);
    
    PT_ASSERT(at(l0, 0) is li0);
    PT_ASSERT(at(l0, 1) is li1);
    PT_ASSERT(at(l0, 2) is li2);
    PT_ASSERT(at(l0, 3) is li3);
    
    set(l0, 0, li4);
    
    PT_ASSERT(at(l0, 0) is li4);
    PT_ASSERT(at(l0, 1) is li1);
    PT_ASSERT(at(l0, 2) is li2);
    PT_ASSERT(at(l0, 3) is li3);
    
    set(l0, 1, li5);
    set(l0, 2, li6);
    set(l0, 3, li0);
    
    PT_ASSERT(at(l0, 0) is li4);
    PT_ASSERT(at(l0, 1) is li5);
    PT_ASSERT(at(l0, 2) is li6);
    PT_ASSERT(at(l0, 3) is li0);
    
    delete(l0);
    
  }

  PT_TEST(test_list_iter) {
    
    TEST_VALUES();
    
    var l0 = new(List, 4, li0, li1, li2, li3);
    
    int counter = 0;
    
    foreach(item in l0) {
      
      switch(counter) {
        case 0: PT_ASSERT(item is li0); break;
        case 1: PT_ASSERT(item is li1); break;
        case 2: PT_ASSERT(item is li2); break;
        case 3: PT_ASSERT(item is li3); break;
      }
      
      counter++;
    }
    
    delete(l0);
    
  }

  PT_TEST(test_list_reverse) {
    
    TEST_VALUES();
    
    var l0 = new(List, 4, li0, li1, li2, li3);
    var l1 = new(List, 4, li3, li2, li1, li0);

    PT_ASSERT(neq(l0, l1));
    
    reverse(l1);
    
    PT_ASSERT(eq(l0, l1));
    
    delete(l0);
    delete(l1);
    
  }

  PT_TEST(test_dictionary_create) {
    
    TEST_VALUES();
    
    var d0 = new(Dictionary);
    put(d0, dk0, dv0);
    put(d0, dk1, dv5);
    put(d0, dk2, dv9);
    
    var d1 = new(Dictionary);
    put(d1, dk3, dv2);
    put(d1, dk4, dv3);
    put(d1, dk5, dv4);
    
    var d2 = copy(d0);
    
    PT_ASSERT(d0);
    PT_ASSERT(d1);
    PT_ASSERT(d2);
    
    PT_ASSERT(d0 isnt d1);
    PT_ASSERT(d0 isnt d2);
    PT_ASSERT(d1 isnt d2);
    
    PT_ASSERT(contains(d0, dk0));
    PT_ASSERT(contains(d0, dk1));
    PT_ASSERT(contains(d0, dk2));
    
    PT_ASSERT(contains(d1, dk3));
    PT_ASSERT(contains(d1, dk4));
    PT_ASSERT(contains(d1, dk5));
    
    PT_ASSERT(contains(d2, dk0));
    PT_ASSERT(contains(d2, dk1));
    PT_ASSERT(contains(d2, dk2));
    
    assign(d2, d1);
    
    PT_ASSERT(contains(d2, dk3));
    PT_ASSERT(contains(d2, dk4));
    PT_ASSERT(contains(d2, dk5));
    
    delete(d0);
    delete(d1);
    delete(d2);
    
  }

  PT_TEST(test_dictionary_collection) {
    
    TEST_VALUES();
    
    var d0 = new(Dictionary);
    put(d0, dk0, dv0);
    put(d0, dk1, dv5);
    put(d0, dk2, dv9);
    
    PT_ASSERT(len(d0) is 3);
    PT_ASSERT(contains(d0, dk0));
    PT_ASSERT(contains(d0, dk1));
    PT_ASSERT(contains(d0, dk2));
    
    discard(d0, dk0);
    
    PT_ASSERT(len(d0) is 2);
    PT_ASSERT(not contains(d0, dk0));
    PT_ASSERT(contains(d0, dk2));
    PT_ASSERT(contains(d0, dk2));

    clear(d0);
    
    PT_ASSERT(len(d0) is 0);
    PT_ASSERT(not contains(d0, dk0));
    PT_ASSERT(not contains(d0, dk2));
    PT_ASSERT(not contains(d0, dk2));
    
    delete(d0);
    
  }

  PT_TEST(test_dictionary_iter) {
    
    TEST_VALUES();
    
    var d0 = new(Dictionary);
    put(d0, dk0, dv0);
    put(d0, dk1, dv5);
    put(d0, dk2, dv9);
    
    int counter = 0;
    
    foreach(key in d0) {
    
      var val = get(d0, key);
      
      switch(counter) {
        case 0: PT_ASSERT(key is dk0); break;
        case 1: PT_ASSERT(key is dk1); break;
        case 2: PT_ASSERT(key is dk2); break;
      }
      
      switch(counter) {
        case 0: PT_ASSERT(val is dv0); break;
        case 1: PT_ASSERT(val is dv5); break;
        case 2: PT_ASSERT(val is dv9); break;
      }
      
      counter++;
    }
    
    PT_ASSERT(counter is 3);
    
  }

  PT_TEST(test_dictionary_dict) {
    
    TEST_VALUES();
    
    var d0 = new(Dictionary);
    put(d0, dk0, dv0);
    put(d0, dk1, dv5);
    put(d0, dk2, dv9);
    
    PT_ASSERT(get(d0, dk0) is dv0);
    PT_ASSERT(get(d0, dk1) is dv5);
    PT_ASSERT(get(d0, dk2) is dv9);
    
    delete(d0);
    
  }

  PT_TEST(test_map_create) {
    
    TEST_VALUES();
    
    var m0 = new(Map);
    put(m0, mk0, dv0);
    put(m0, mk1, dv5);
    put(m0, mk2, dv9);
    
    var m1 = new(Map);
    put(m1, mk3, dv2);
    put(m1, mk4, dv3);
    put(m1, mk5, dv4);
    
    var m2 = copy(m0);
    
    PT_ASSERT(m0);
    PT_ASSERT(m1);
    PT_ASSERT(m2);
    
    PT_ASSERT(m0 isnt m1);
    PT_ASSERT(m0 isnt m2);
    PT_ASSERT(m1 isnt m2);
    
    PT_ASSERT(contains(m0, mk0));
    PT_ASSERT(contains(m0, mk1));
    PT_ASSERT(contains(m0, mk2));
    
    PT_ASSERT(contains(m1, mk3));
    PT_ASSERT(contains(m1, mk4));
    PT_ASSERT(contains(m1, mk5));
    
    PT_ASSERT(contains(m2, mk0));
    PT_ASSERT(contains(m2, mk1));
    PT_ASSERT(contains(m2, mk2));
    
    assign(m2, m1);
    
    PT_ASSERT(contains(m2, mk3));
    PT_ASSERT(contains(m2, mk4));
    PT_ASSERT(contains(m2, mk5));
    
    delete(m0);
    delete(m1);
    delete(m2);
    
  }

  PT_TEST(test_map_collection) {
    
    TEST_VALUES();
    
    var m0 = new(Map);
    put(m0, mk0, dv0);
    put(m0, mk1, dv5);
    put(m0, mk2, dv9);
    
    PT_ASSERT(len(m0) is 3);
    PT_ASSERT(contains(m0, mk0));
    PT_ASSERT(contains(m0, mk1));
    PT_ASSERT(contains(m0, mk2));
    
    discard(m0, mk0);
    
    PT_ASSERT(len(m0) is 2);
    PT_ASSERT(not contains(m0, mk0));
    PT_ASSERT(contains(m0, mk2));
    PT_ASSERT(contains(m0, mk2));

    clear(m0);
    
    PT_ASSERT(len(m0) is 0);
    PT_ASSERT(not contains(m0, mk0));
    PT_ASSERT(not contains(m0, mk2));
    PT_ASSERT(not contains(m0, mk2));
    
    delete(m0);
    
  }

  PT_TEST(test_map_iter) {
    
    TEST_VALUES();
    
    var m0 = new(Map);
    put(m0, mk0, dv0);
    put(m0, mk1, dv5);
    put(m0, mk2, dv9);
    
    int counter = 0;
    
    foreach(key in m0) {
    
      var val = get(m0, key);
      
      switch(counter) {
        case 0: PT_ASSERT(key is mk0); break;
        case 1: PT_ASSERT(key is mk1); break;
        case 2: PT_ASSERT(key is mk2); break;
      }
      
      switch(counter) {
        case 0: PT_ASSERT(val is dv0); break;
        case 1: PT_ASSERT(val is dv5); break;
        case 2: PT_ASSERT(val is dv9); break;
      }
      
      counter++;
    }
    
    PT_ASSERT(counter is 3);
    
  }

  PT_TEST(test_map_dict) {
    
    TEST_VALUES();
    
    var m0 = new(Map);
    put(m0, mk0, dv0);
    put(m0, mk1, dv5);
    put(m0, mk2, dv9);
    
    PT_ASSERT(get(m0, mk0) is dv0);
    PT_ASSERT(get(m0, mk1) is dv5);
    PT_ASSERT(get(m0, mk2) is dv9);
    
    delete(m0);
    
  }

  PT_TEST(test_file_create) {
    
    TEST_VALUES();
    
    var f0 = new(File, "test.bin", "w"); 
    var f1 = $(File, NULL);
    
    PT_ASSERT(f0);
    PT_ASSERT(f1);
    
    PT_ASSERT(f0 isnt f1);
    
    delete(f0);
    
  }

  local char testoutput1[] = "This is a test\n";
  local char testoutput2[] = "This is a sample\n";
  local char testinput[512];

  PT_TEST(test_file_read) {
    
    var f0 = $(File, NULL);
    
    open(f0, "test.txt", "w");
    write(f0, testoutput1, sizeof(testoutput1));
    close(f0);  
    
    open(f0, "test.txt", "r");
    read(f0, testinput, sizeof(testoutput1));
    close(f0);
    
    PT_ASSERT_STR_EQ(testinput, testoutput1);
    
  }

  PT_TEST(test_file_dict) {
    
    var f = $(File, NULL);
    
    open(f, "test.bin", "w");
      
      put(f, Int, $(Int, 1));
      put(f, Int, $(Int, 22));
    
    close(f);
    
    open(f, "test.bin", "r");
      
      var first = get(f, Int);
      var second = get(f, Int);
      
      PT_ASSERT( eq(first, $(Int, 1)) );
      PT_ASSERT( eq(second, $(Int, 22)) );
      
      delete(first);
      delete(second);
      
    close(f);
    
  }

}

cmake_minimum_required(VERSION 3.8.0)

project(Cello LANGUAGES C VERSION 2.1.0)

set(CMAKE_C_STANDARD 99)

###############################################################################
# CMake Options
###############################################################################

option(CELLO_BUILD_TESTS OFF)
option(CELLO_BUILD_EXAMPLES OFF)

###############################################################################
# Find Threads
###############################################################################

find_package(Threads REQUIRED)

###############################################################################
# Add Cello Library
###############################################################################

set(SOURCES src/Alloc.c
            src/Array.c
            src/Assign.c
            src/Cmp.c
            src/Concat.c
            src/Doc.c
            src/Exception.c
            src/File.c
            src/Function.c
            src/GC.c
            src/Get.c
            src/Hash.c
            src/Iter.c
            src/Len.c
            src/List.c
            src/Num.c
            src/Pointer.c
            src/Push.c
            src/Resize.c
            src/Show.c
            src/Start.c
            src/String.c
            src/Table.c
            src/Thread.c
            src/Tree.c
            src/Tuple.c
            src/Type.c)

add_library(Cello ${SOURCES})
add_library(Cello::Cello ALIAS Cello)

target_include_directories(Cello PUBLIC include)

target_link_libraries(Cello PRIVATE Threads::Threads)
target_link_libraries(Cello PRIVATE m)

###############################################################################
# Build Tests
###############################################################################

if(CELLO_BUILD_TESTS)
    add_subdirectory(vendor/PTest)
	enable_testing()
	macro(cello_add_test TESTNAME)
		add_executable(${TESTNAME} ${ARGN})
		target_link_libraries(${TESTNAME} PRIVATE Cello::Cello)
		target_link_libraries(${TESTNAME} PRIVATE PTest::PTest)
		add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
		set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
	endmacro()
	add_subdirectory(tests)
endif()

###############################################################################
# Build Examples
###############################################################################

if(CELLO_BUILD_EXAMPLES)
	macro(cello_add_example EXAMPLENAME)
		add_executable(${EXAMPLENAME} ${ARGN})
		target_link_libraries(${EXAMPLENAME} PRIVATE Cello::Cello)
		set_target_properties(${EXAMPLENAME} PROPERTIES FOLDER examples)
	endmacro()
	add_subdirectory(examples)
endif()

IF(MSVC)
	MESSAGE(FATAL_ERROR "Libcello not supported in Visual")
ENDIF()

INIT_BIICODE_BLOCK()

# Make sure that the test source file are not in the library
LIST(REMOVE_ITEM BII_LIB_SRC tests/ptest.c tests/ptest.h)
SET(BII_tests_test_SRC ${BII_tests_test_SRC} 	tests/ptest.c
												tests/ptest.h
												tests/core.c
												tests/data.c
												tests/functional.c
												tests/memory.c
												tests/exceptions.c
												tests/threading.c)
ADD_BIICODE_TARGETS()

# Extra configuration
TARGET_COMPILE_OPTIONS(${BII_BLOCK_TARGET} INTERFACE "-std=gnu99")
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp PROPERTIES COMPILE_FLAGS -std=c++03)
IF(UNIX)
	TARGET_LINK_LIBRARIES(${BII_BLOCK_TARGET} INTERFACE -lpthread -ldl -lm)
ENDIF()

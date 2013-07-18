FILE(GLOB_RECURSE
    TESTS_SRC
        core.c
        data.c
        exception.c
        functional.c
        memory.c
        ptest.c
        test.c
    )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE(tests ${TESTS_SRC})

TARGET_LINK_LIBRARIES(tests ${CELLO_LIBRARY_TARGET})


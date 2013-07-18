CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

PROJECT(Cello C)

SET(Cello_VERSION_MAJOR 0)
SET(Cello_VERSION_MINOR 9)
SET(Cello_VERSION_PATCH 3)
SET(Cello_VERSION_STRING
    ${Cello_VERSION_MAJOR}.${Cello_VERSION_MINOR}.${Cello_VERSION_PATCH})

INCLUDE(configure)
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/config.h.cmake ${PROJECT_SOURCE_DIR}/config.h)

SET(CMAKE_BUILD_TYPE Debug)

FILE(GLOB
    CELLO_SRC
        ${PROJECT_SOURCE_DIR}/src/Array.c
        ${PROJECT_SOURCE_DIR}/src/Bool.c
        ${PROJECT_SOURCE_DIR}/src/Char.c
        ${PROJECT_SOURCE_DIR}/src/Dictionary.c
        ${PROJECT_SOURCE_DIR}/src/Exception.c
        ${PROJECT_SOURCE_DIR}/src/File.c
        ${PROJECT_SOURCE_DIR}/src/Format.c
        ${PROJECT_SOURCE_DIR}/src/Function.c
        ${PROJECT_SOURCE_DIR}/src/Lambda.c
        ${PROJECT_SOURCE_DIR}/src/List.c
        ${PROJECT_SOURCE_DIR}/src/Map.c
        ${PROJECT_SOURCE_DIR}/src/Number.c
        ${PROJECT_SOURCE_DIR}/src/Pool.c
        ${PROJECT_SOURCE_DIR}/src/Prelude.c
        ${PROJECT_SOURCE_DIR}/src/Reference.c
        ${PROJECT_SOURCE_DIR}/src/String.c
        ${PROJECT_SOURCE_DIR}/src/Table.c
        ${PROJECT_SOURCE_DIR}/src/Tree.c
        ${PROJECT_SOURCE_DIR}/src/Type.c
    )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

ADD_DEFINITIONS(-std=gnu99 -Wall -Wextra -Wno-unused)

ADD_LIBRARY(Cello SHARED ${CELLO_SRC})
SET_TARGET_PROPERTIES(Cello PROPERTIES VERSION ${Cello_VERSION_STRING})

TARGET_LINK_LIBRARIES(Cello m)


FILE(GLOB
    CELLO_HEADERS
    ${PROJECT_SOURCE_DIR}/include/Cello.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Array.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Bool.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Char.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Dictionary.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Exception.h
    ${PROJECT_SOURCE_DIR}/include/Cello/File.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Format.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Function.h
    ${PROJECT_SOURCE_DIR}/include/FunctionAutoCello/.h 
    ${PROJECT_SOURCE_DIR}/include/Cello/Lambda.h
    ${PROJECT_SOURCE_DIR}/include/Cello/LambdaCurry.h
    ${PROJECT_SOURCE_DIR}/include/Cello/List.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Map.h
    ${PROJECT_SOURCE_DIR}/include/Cello/None.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Number.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Pool.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Prelude.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Reference.h
    ${PROJECT_SOURCE_DIR}/include/Cello/String.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Table.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Tree.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Type.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Value.h
    )

INSTALL(TARGETS Cello DESTINATION lib)
INSTALL(FILES ${CELLO_HEADERS} DESTINATION include)

ADD_SUBDIRECTORY(tests)

ADD_SUBDIRECTORY(demos)

SET(USE_VIM_CLANG_COMPLETE YES CACHE BOOL "If set, will generate a .clang_complete file used by Vim clang_complete plugin")
IF(USE_VIM_CLANG_COMPLETE)
################################################
# GENERATE VIM CLANG_COMPLETE FILE
################################################
GET_PROPERTY(PROJECT_HEADERS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests PROPERTY INCLUDE_DIRECTORIES)
    FILE(WRITE .clang_complete "")
    FOREACH(PATH ${PROJECT_HEADERS})
        FILE(APPEND .clang_complete "-I${PATH}\n")
    ENDFOREACH(PATH)
ENDIF()


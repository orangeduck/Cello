cmake_minimum_required(VERSION 3.8.0)

project(PTest LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(SOURCES src/PTest.c)

add_library(PTest ${SOURCES})
add_library(PTest::PTest ALIAS PTest)

target_include_directories(PTest PUBLIC include)
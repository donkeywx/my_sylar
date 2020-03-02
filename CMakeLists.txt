cmake_minimum_required(VERSION 3.0)
SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(sylar)

SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function") 

SET(LIB_SRC
    sylar/log.cpp
    sylar/log.h
)

add_library(sylar SHARED ${LIB_SRC})
add_executable(testLog tests/test.cpp)
add_dependencies(testLog sylar)
target_link_libraries(testLog sylar)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

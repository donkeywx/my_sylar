cmake_minimum_required(VERSION 3.0)
SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(sylar)

SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function") 

SET(LIB_SRC
    sylar/log.cpp
    sylar/util.cpp
    sylar/config.cpp
)
LINK_DIRECTORIES(/usr/local/yaml-cpp/lib)
INCLUDE_DIRECTORIES(/usr/local/yaml-cpp/include)
LINK_DIRECTORIES(/usr/local/boost/lib)
INCLUDE_DIRECTORIES(/usr/local/boost/include)


add_library(sylar SHARED ${LIB_SRC})

add_executable(testLog tests/test.cpp)
add_dependencies(testLog sylar)
target_link_libraries(testLog sylar -lyaml-cpp)

add_executable(testConfig tests/test_config.cpp)
add_dependencies(testConfig sylar)
target_link_libraries(testConfig sylar -lyaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
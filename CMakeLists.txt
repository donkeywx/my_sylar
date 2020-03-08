
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(sylar)

SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -g -std=c++11 -Wall -Wno-builtin-macro-redefined -Wno-deprecated -Werror -Wno-unused-function") 
SET(LIB_SRC
    sylar/log.cpp
    sylar/util.cpp
    sylar/config.cpp
    sylar/thread.cpp
)
SET(LIB_LIB
    sylar
    pthread
    yaml-cpp)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

INCLUDE(cmake/utils.cmake)
LINK_DIRECTORIES(/usr/local/yaml-cpp/lib)
LINK_DIRECTORIES(/usr/local/boost/lib)
INCLUDE_DIRECTORIES(/usr/local/yaml-cpp/include)
INCLUDE_DIRECTORIES(/usr/local/boost/include)

ADD_LIBRARY(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar) ## redefine __FILE__

ADD_EXECUTABLE(testLog tests/testLog.cpp)
ADD_DEPENDENCIES(testLog sylar)
force_redefine_file_macro_for_sources(testLog)
TARGET_LINK_LIBRARIES(testLog ${LIB_LIB})

ADD_EXECUTABLE(testConfig tests/testConfig.cpp)
ADD_DEPENDENCIES(testConfig sylar)
force_redefine_file_macro_for_sources(testConfig)
TARGET_LINK_LIBRARIES(testConfig ${LIB_LIB})

ADD_EXECUTABLE(testThread tests/testThread.cpp)
ADD_DEPENDENCIES(testThread sylar)
force_redefine_file_macro_for_sources(testThread)
TARGET_LINK_LIBRARIES(testThread ${LIB_LIB})
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(sylar)

SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET(CMAKE_VERBOSE_MAKEFILE ON)
# -fsanitize=address
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -g  -std=c++11 -Wall -Wno-unused-const-variable -Wno-builtin-macro-redefined -Wno-deprecated -Werror -Wno-unused-function") 
SET(LIB_SRC
    sylar/log.cpp
    sylar/util.cpp
    sylar/config.cpp
    sylar/thread.cpp
    sylar/mutex.cpp
    sylar/macro.cpp
    sylar/fiber.cpp
    sylar/scheduler.cpp
    sylar/iomanager.cpp
    sylar/timer.cpp
    sylar/hook.cpp
    sylar/fdmanager.cpp
    sylar/address.cpp
    sylar/socket.cpp
    sylar/tcpserver.cpp
    sylar/stream.cpp
    sylar/socket_stream.cpp
    sylar/bytearray.cpp
    sylar/uri.cc
    #sylar/zlib_stream.cpp
    sylar/http/http.cpp
    sylar/http/http_parser.cpp
    sylar/http/http11_parser.cc
    sylar/http/httpclient_parser.cc
    sylar/http/http_session.cpp
    sylar/http/http_server.cpp
    sylar/http/servlet.cpp
    #sylar/http/http_connection.cpp
)
SET(LIB_LIB
    sylar
    pthread
    yaml-cpp
    dl)
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

ADD_EXECUTABLE(testStack tests/testStack.cpp)
ADD_DEPENDENCIES(testStack sylar)
force_redefine_file_macro_for_sources(testStack)
TARGET_LINK_LIBRARIES(testStack ${LIB_LIB})

ADD_EXECUTABLE(testFiber tests/testFiber.cpp)
ADD_DEPENDENCIES(testFiber sylar)
force_redefine_file_macro_for_sources(testFiber)
TARGET_LINK_LIBRARIES(testFiber ${LIB_LIB})

ADD_EXECUTABLE(testScheduler tests/testScheduler.cpp)
ADD_DEPENDENCIES(testScheduler sylar)
force_redefine_file_macro_for_sources(testScheduler)
TARGET_LINK_LIBRARIES(testScheduler ${LIB_LIB})

ADD_EXECUTABLE(testIomanager tests/testIomanager.cpp)
ADD_DEPENDENCIES(testIomanager sylar)
force_redefine_file_macro_for_sources(testIomanager)
TARGET_LINK_LIBRARIES(testIomanager ${LIB_LIB})

ADD_EXECUTABLE(testHook tests/testHook.cpp)
ADD_DEPENDENCIES(testHook sylar)
force_redefine_file_macro_for_sources(testHook)
TARGET_LINK_LIBRARIES(testHook ${LIB_LIB})

ADD_EXECUTABLE(testAddress tests/testAddress.cpp)
ADD_DEPENDENCIES(testAddress sylar)
force_redefine_file_macro_for_sources(testAddress)
TARGET_LINK_LIBRARIES(testAddress ${LIB_LIB})

ADD_EXECUTABLE(testSocket tests/testSocket.cpp)
ADD_DEPENDENCIES(testSocket sylar)
force_redefine_file_macro_for_sources(testSocket)
TARGET_LINK_LIBRARIES(testSocket ${LIB_LIB})

ADD_EXECUTABLE(testByteArray tests/testByteArray.cpp)
ADD_DEPENDENCIES(testByteArray sylar)
force_redefine_file_macro_for_sources(testByteArray)
TARGET_LINK_LIBRARIES(testByteArray ${LIB_LIB})

ADD_EXECUTABLE(testHttp tests/testHttp.cpp)
ADD_DEPENDENCIES(testHttp sylar)
force_redefine_file_macro_for_sources(testHttp)
TARGET_LINK_LIBRARIES(testHttp ${LIB_LIB})

ADD_EXECUTABLE(testHttpParse tests/testHttpParse.cpp)
ADD_DEPENDENCIES(testHttpParse sylar)
force_redefine_file_macro_for_sources(testHttpParse)
TARGET_LINK_LIBRARIES(testHttpParse ${LIB_LIB})

ADD_EXECUTABLE(testHttpServer tests/testHttpServer.cpp)
ADD_DEPENDENCIES(testHttpServer sylar)
force_redefine_file_macro_for_sources(testHttpServer)
TARGET_LINK_LIBRARIES(testHttpServer ${LIB_LIB})

ADD_EXECUTABLE(testTcpServer tests/testTcpServer.cpp)
ADD_DEPENDENCIES(testTcpServer sylar)
force_redefine_file_macro_for_sources(testTcpServer)
TARGET_LINK_LIBRARIES(testTcpServer ${LIB_LIB})

#ADD_EXECUTABLE(testConnection tests/testConnection.cpp)
#ADD_DEPENDENCIES(testConnection sylar)
#force_redefine_file_macro_for_sources(testConnection)
#TARGET_LINK_LIBRARIES(testConnection ${LIB_LIB})
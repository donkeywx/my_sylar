# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wangk/code/cpp/sylar/code/my_sylar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wangk/code/cpp/sylar/code/my_sylar

# Include any dependencies generated for this target.
include CMakeFiles/testBoostContext.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testBoostContext.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testBoostContext.dir/flags.make

CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o: CMakeFiles/testBoostContext.dir/flags.make
CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o: tests/testBoostContext.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangk/code/cpp/sylar/code/my_sylar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) -D__FILE__=\"tests/testBoostContext.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o -c /home/wangk/code/cpp/sylar/code/my_sylar/tests/testBoostContext.cpp

CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) -D__FILE__=\"tests/testBoostContext.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangk/code/cpp/sylar/code/my_sylar/tests/testBoostContext.cpp > CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.i

CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) -D__FILE__=\"tests/testBoostContext.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangk/code/cpp/sylar/code/my_sylar/tests/testBoostContext.cpp -o CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.s

CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o.requires:

.PHONY : CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o.requires

CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o.provides: CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o.requires
	$(MAKE) -f CMakeFiles/testBoostContext.dir/build.make CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o.provides.build
.PHONY : CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o.provides

CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o.provides.build: CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o


# Object files for target testBoostContext
testBoostContext_OBJECTS = \
"CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o"

# External object files for target testBoostContext
testBoostContext_EXTERNAL_OBJECTS =

bin/testBoostContext: CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o
bin/testBoostContext: CMakeFiles/testBoostContext.dir/build.make
bin/testBoostContext: lib/libsylar.so
bin/testBoostContext: CMakeFiles/testBoostContext.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wangk/code/cpp/sylar/code/my_sylar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/testBoostContext"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testBoostContext.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testBoostContext.dir/build: bin/testBoostContext

.PHONY : CMakeFiles/testBoostContext.dir/build

CMakeFiles/testBoostContext.dir/requires: CMakeFiles/testBoostContext.dir/tests/testBoostContext.cpp.o.requires

.PHONY : CMakeFiles/testBoostContext.dir/requires

CMakeFiles/testBoostContext.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testBoostContext.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testBoostContext.dir/clean

CMakeFiles/testBoostContext.dir/depend:
	cd /home/wangk/code/cpp/sylar/code/my_sylar && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar/CMakeFiles/testBoostContext.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testBoostContext.dir/depend


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
include CMakeFiles/testFiber.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testFiber.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testFiber.dir/flags.make

CMakeFiles/testFiber.dir/tests/testFiber.cpp.o: CMakeFiles/testFiber.dir/flags.make
CMakeFiles/testFiber.dir/tests/testFiber.cpp.o: tests/testFiber.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangk/code/cpp/sylar/code/my_sylar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testFiber.dir/tests/testFiber.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) -D__FILE__=\"tests/testFiber.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testFiber.dir/tests/testFiber.cpp.o -c /home/wangk/code/cpp/sylar/code/my_sylar/tests/testFiber.cpp

CMakeFiles/testFiber.dir/tests/testFiber.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testFiber.dir/tests/testFiber.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) -D__FILE__=\"tests/testFiber.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangk/code/cpp/sylar/code/my_sylar/tests/testFiber.cpp > CMakeFiles/testFiber.dir/tests/testFiber.cpp.i

CMakeFiles/testFiber.dir/tests/testFiber.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testFiber.dir/tests/testFiber.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) -D__FILE__=\"tests/testFiber.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangk/code/cpp/sylar/code/my_sylar/tests/testFiber.cpp -o CMakeFiles/testFiber.dir/tests/testFiber.cpp.s

CMakeFiles/testFiber.dir/tests/testFiber.cpp.o.requires:

.PHONY : CMakeFiles/testFiber.dir/tests/testFiber.cpp.o.requires

CMakeFiles/testFiber.dir/tests/testFiber.cpp.o.provides: CMakeFiles/testFiber.dir/tests/testFiber.cpp.o.requires
	$(MAKE) -f CMakeFiles/testFiber.dir/build.make CMakeFiles/testFiber.dir/tests/testFiber.cpp.o.provides.build
.PHONY : CMakeFiles/testFiber.dir/tests/testFiber.cpp.o.provides

CMakeFiles/testFiber.dir/tests/testFiber.cpp.o.provides.build: CMakeFiles/testFiber.dir/tests/testFiber.cpp.o


# Object files for target testFiber
testFiber_OBJECTS = \
"CMakeFiles/testFiber.dir/tests/testFiber.cpp.o"

# External object files for target testFiber
testFiber_EXTERNAL_OBJECTS =

bin/testFiber: CMakeFiles/testFiber.dir/tests/testFiber.cpp.o
bin/testFiber: CMakeFiles/testFiber.dir/build.make
bin/testFiber: lib/libsylar.so
bin/testFiber: CMakeFiles/testFiber.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wangk/code/cpp/sylar/code/my_sylar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/testFiber"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testFiber.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testFiber.dir/build: bin/testFiber

.PHONY : CMakeFiles/testFiber.dir/build

CMakeFiles/testFiber.dir/requires: CMakeFiles/testFiber.dir/tests/testFiber.cpp.o.requires

.PHONY : CMakeFiles/testFiber.dir/requires

CMakeFiles/testFiber.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testFiber.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testFiber.dir/clean

CMakeFiles/testFiber.dir/depend:
	cd /home/wangk/code/cpp/sylar/code/my_sylar && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar/CMakeFiles/testFiber.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testFiber.dir/depend


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
include CMakeFiles/testIomanager.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testIomanager.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testIomanager.dir/flags.make

CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o: CMakeFiles/testIomanager.dir/flags.make
CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o: tests/testIomanager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangk/code/cpp/sylar/code/my_sylar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) -D__FILE__=\"tests/testIomanager.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o -c /home/wangk/code/cpp/sylar/code/my_sylar/tests/testIomanager.cpp

CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) -D__FILE__=\"tests/testIomanager.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangk/code/cpp/sylar/code/my_sylar/tests/testIomanager.cpp > CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.i

CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) -D__FILE__=\"tests/testIomanager.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangk/code/cpp/sylar/code/my_sylar/tests/testIomanager.cpp -o CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.s

CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o.requires:

.PHONY : CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o.requires

CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o.provides: CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o.requires
	$(MAKE) -f CMakeFiles/testIomanager.dir/build.make CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o.provides.build
.PHONY : CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o.provides

CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o.provides.build: CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o


# Object files for target testIomanager
testIomanager_OBJECTS = \
"CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o"

# External object files for target testIomanager
testIomanager_EXTERNAL_OBJECTS =

bin/testIomanager: CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o
bin/testIomanager: CMakeFiles/testIomanager.dir/build.make
bin/testIomanager: lib/libsylar.so
bin/testIomanager: CMakeFiles/testIomanager.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wangk/code/cpp/sylar/code/my_sylar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/testIomanager"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testIomanager.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testIomanager.dir/build: bin/testIomanager

.PHONY : CMakeFiles/testIomanager.dir/build

CMakeFiles/testIomanager.dir/requires: CMakeFiles/testIomanager.dir/tests/testIomanager.cpp.o.requires

.PHONY : CMakeFiles/testIomanager.dir/requires

CMakeFiles/testIomanager.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testIomanager.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testIomanager.dir/clean

CMakeFiles/testIomanager.dir/depend:
	cd /home/wangk/code/cpp/sylar/code/my_sylar && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar /home/wangk/code/cpp/sylar/code/my_sylar/CMakeFiles/testIomanager.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testIomanager.dir/depend

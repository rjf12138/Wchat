# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ruanjian/workspace/project/Wchat/Wchat_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ruanjian/workspace/project/Wchat/Wchat_server/build

# Include any dependencies generated for this target.
include src/CMakeFiles/thread_pool.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/thread_pool.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/thread_pool.dir/flags.make

src/CMakeFiles/thread_pool.dir/thread_pool.cc.o: src/CMakeFiles/thread_pool.dir/flags.make
src/CMakeFiles/thread_pool.dir/thread_pool.cc.o: ../src/thread_pool.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ruanjian/workspace/project/Wchat/Wchat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/thread_pool.dir/thread_pool.cc.o"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/thread_pool.dir/thread_pool.cc.o -c /home/ruanjian/workspace/project/Wchat/Wchat_server/src/thread_pool.cc

src/CMakeFiles/thread_pool.dir/thread_pool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thread_pool.dir/thread_pool.cc.i"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ruanjian/workspace/project/Wchat/Wchat_server/src/thread_pool.cc > CMakeFiles/thread_pool.dir/thread_pool.cc.i

src/CMakeFiles/thread_pool.dir/thread_pool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thread_pool.dir/thread_pool.cc.s"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ruanjian/workspace/project/Wchat/Wchat_server/src/thread_pool.cc -o CMakeFiles/thread_pool.dir/thread_pool.cc.s

# Object files for target thread_pool
thread_pool_OBJECTS = \
"CMakeFiles/thread_pool.dir/thread_pool.cc.o"

# External object files for target thread_pool
thread_pool_EXTERNAL_OBJECTS =

src/lib/libthread_pool.so: src/CMakeFiles/thread_pool.dir/thread_pool.cc.o
src/lib/libthread_pool.so: src/CMakeFiles/thread_pool.dir/build.make
src/lib/libthread_pool.so: src/CMakeFiles/thread_pool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ruanjian/workspace/project/Wchat/Wchat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library lib/libthread_pool.so"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/thread_pool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/thread_pool.dir/build: src/lib/libthread_pool.so

.PHONY : src/CMakeFiles/thread_pool.dir/build

src/CMakeFiles/thread_pool.dir/clean:
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && $(CMAKE_COMMAND) -P CMakeFiles/thread_pool.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/thread_pool.dir/clean

src/CMakeFiles/thread_pool.dir/depend:
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ruanjian/workspace/project/Wchat/Wchat_server /home/ruanjian/workspace/project/Wchat/Wchat_server/src /home/ruanjian/workspace/project/Wchat/Wchat_server/build /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src/CMakeFiles/thread_pool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/thread_pool.dir/depend


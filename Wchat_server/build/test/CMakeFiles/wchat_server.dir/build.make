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
include test/CMakeFiles/wchat_server.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/wchat_server.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/wchat_server.dir/flags.make

test/CMakeFiles/wchat_server.dir/wchat_server.cc.o: test/CMakeFiles/wchat_server.dir/flags.make
test/CMakeFiles/wchat_server.dir/wchat_server.cc.o: ../test/wchat_server.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ruanjian/workspace/project/Wchat/Wchat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/wchat_server.dir/wchat_server.cc.o"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wchat_server.dir/wchat_server.cc.o -c /home/ruanjian/workspace/project/Wchat/Wchat_server/test/wchat_server.cc

test/CMakeFiles/wchat_server.dir/wchat_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wchat_server.dir/wchat_server.cc.i"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ruanjian/workspace/project/Wchat/Wchat_server/test/wchat_server.cc > CMakeFiles/wchat_server.dir/wchat_server.cc.i

test/CMakeFiles/wchat_server.dir/wchat_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wchat_server.dir/wchat_server.cc.s"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ruanjian/workspace/project/Wchat/Wchat_server/test/wchat_server.cc -o CMakeFiles/wchat_server.dir/wchat_server.cc.s

# Object files for target wchat_server
wchat_server_OBJECTS = \
"CMakeFiles/wchat_server.dir/wchat_server.cc.o"

# External object files for target wchat_server
wchat_server_EXTERNAL_OBJECTS =

test/bin/wchat_server: test/CMakeFiles/wchat_server.dir/wchat_server.cc.o
test/bin/wchat_server: test/CMakeFiles/wchat_server.dir/build.make
test/bin/wchat_server: src/lib/libaccount_manager.so
test/bin/wchat_server: src/lib/libserver_task.so
test/bin/wchat_server: src/lib/libmsg_handler.so
test/bin/wchat_server: src/lib/libmsg_buffer.so
test/bin/wchat_server: src/lib/libqueue.so
test/bin/wchat_server: src/lib/libuser_manager.so
test/bin/wchat_server: src/lib/libgroup_manager.so
test/bin/wchat_server: src/lib/libsqlitewrapper.so
test/bin/wchat_server: src/lib/libserverlog.so
test/bin/wchat_server: src/lib/libserverjson.so
test/bin/wchat_server: src/lib/libmsg_transmission.so
test/bin/wchat_server: src/lib/libserver_signal.so
test/bin/wchat_server: src/lib/libconnect_manager.so
test/bin/wchat_server: src/lib/libthread_pool.so
test/bin/wchat_server: test/CMakeFiles/wchat_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ruanjian/workspace/project/Wchat/Wchat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/wchat_server"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wchat_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/wchat_server.dir/build: test/bin/wchat_server

.PHONY : test/CMakeFiles/wchat_server.dir/build

test/CMakeFiles/wchat_server.dir/clean:
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/test && $(CMAKE_COMMAND) -P CMakeFiles/wchat_server.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/wchat_server.dir/clean

test/CMakeFiles/wchat_server.dir/depend:
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ruanjian/workspace/project/Wchat/Wchat_server /home/ruanjian/workspace/project/Wchat/Wchat_server/test /home/ruanjian/workspace/project/Wchat/Wchat_server/build /home/ruanjian/workspace/project/Wchat/Wchat_server/build/test /home/ruanjian/workspace/project/Wchat/Wchat_server/build/test/CMakeFiles/wchat_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/wchat_server.dir/depend

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
include src/CMakeFiles/sqlitewrapper.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/sqlitewrapper.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/sqlitewrapper.dir/flags.make

src/CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.o: src/CMakeFiles/sqlitewrapper.dir/flags.make
src/CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.o: ../src/sqlite_wrapper.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ruanjian/workspace/project/Wchat/Wchat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.o"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.o -c /home/ruanjian/workspace/project/Wchat/Wchat_server/src/sqlite_wrapper.cc

src/CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.i"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ruanjian/workspace/project/Wchat/Wchat_server/src/sqlite_wrapper.cc > CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.i

src/CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.s"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ruanjian/workspace/project/Wchat/Wchat_server/src/sqlite_wrapper.cc -o CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.s

# Object files for target sqlitewrapper
sqlitewrapper_OBJECTS = \
"CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.o"

# External object files for target sqlitewrapper
sqlitewrapper_EXTERNAL_OBJECTS =

src/lib/libsqlitewrapper.so: src/CMakeFiles/sqlitewrapper.dir/sqlite_wrapper.cc.o
src/lib/libsqlitewrapper.so: src/CMakeFiles/sqlitewrapper.dir/build.make
src/lib/libsqlitewrapper.so: src/CMakeFiles/sqlitewrapper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ruanjian/workspace/project/Wchat/Wchat_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library lib/libsqlitewrapper.so"
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sqlitewrapper.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/sqlitewrapper.dir/build: src/lib/libsqlitewrapper.so

.PHONY : src/CMakeFiles/sqlitewrapper.dir/build

src/CMakeFiles/sqlitewrapper.dir/clean:
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src && $(CMAKE_COMMAND) -P CMakeFiles/sqlitewrapper.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/sqlitewrapper.dir/clean

src/CMakeFiles/sqlitewrapper.dir/depend:
	cd /home/ruanjian/workspace/project/Wchat/Wchat_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ruanjian/workspace/project/Wchat/Wchat_server /home/ruanjian/workspace/project/Wchat/Wchat_server/src /home/ruanjian/workspace/project/Wchat/Wchat_server/build /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src /home/ruanjian/workspace/project/Wchat/Wchat_server/build/src/CMakeFiles/sqlitewrapper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/sqlitewrapper.dir/depend


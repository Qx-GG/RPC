# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/qinxin/m_rpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/qinxin/m_rpc

# Include any dependencies generated for this target.
include rpc/callee/CMakeFiles/provider.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include rpc/callee/CMakeFiles/provider.dir/compiler_depend.make

# Include the progress variables for this target.
include rpc/callee/CMakeFiles/provider.dir/progress.make

# Include the compile flags for this target's objects.
include rpc/callee/CMakeFiles/provider.dir/flags.make

rpc/callee/CMakeFiles/provider.dir/friendservice.cc.o: rpc/callee/CMakeFiles/provider.dir/flags.make
rpc/callee/CMakeFiles/provider.dir/friendservice.cc.o: rpc/callee/friendservice.cc
rpc/callee/CMakeFiles/provider.dir/friendservice.cc.o: rpc/callee/CMakeFiles/provider.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qinxin/m_rpc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object rpc/callee/CMakeFiles/provider.dir/friendservice.cc.o"
	cd /home/qinxin/m_rpc/rpc/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT rpc/callee/CMakeFiles/provider.dir/friendservice.cc.o -MF CMakeFiles/provider.dir/friendservice.cc.o.d -o CMakeFiles/provider.dir/friendservice.cc.o -c /home/qinxin/m_rpc/rpc/callee/friendservice.cc

rpc/callee/CMakeFiles/provider.dir/friendservice.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/provider.dir/friendservice.cc.i"
	cd /home/qinxin/m_rpc/rpc/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qinxin/m_rpc/rpc/callee/friendservice.cc > CMakeFiles/provider.dir/friendservice.cc.i

rpc/callee/CMakeFiles/provider.dir/friendservice.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/provider.dir/friendservice.cc.s"
	cd /home/qinxin/m_rpc/rpc/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qinxin/m_rpc/rpc/callee/friendservice.cc -o CMakeFiles/provider.dir/friendservice.cc.s

rpc/callee/CMakeFiles/provider.dir/__/friends.pb.cc.o: rpc/callee/CMakeFiles/provider.dir/flags.make
rpc/callee/CMakeFiles/provider.dir/__/friends.pb.cc.o: rpc/friends.pb.cc
rpc/callee/CMakeFiles/provider.dir/__/friends.pb.cc.o: rpc/callee/CMakeFiles/provider.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qinxin/m_rpc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object rpc/callee/CMakeFiles/provider.dir/__/friends.pb.cc.o"
	cd /home/qinxin/m_rpc/rpc/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT rpc/callee/CMakeFiles/provider.dir/__/friends.pb.cc.o -MF CMakeFiles/provider.dir/__/friends.pb.cc.o.d -o CMakeFiles/provider.dir/__/friends.pb.cc.o -c /home/qinxin/m_rpc/rpc/friends.pb.cc

rpc/callee/CMakeFiles/provider.dir/__/friends.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/provider.dir/__/friends.pb.cc.i"
	cd /home/qinxin/m_rpc/rpc/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qinxin/m_rpc/rpc/friends.pb.cc > CMakeFiles/provider.dir/__/friends.pb.cc.i

rpc/callee/CMakeFiles/provider.dir/__/friends.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/provider.dir/__/friends.pb.cc.s"
	cd /home/qinxin/m_rpc/rpc/callee && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qinxin/m_rpc/rpc/friends.pb.cc -o CMakeFiles/provider.dir/__/friends.pb.cc.s

# Object files for target provider
provider_OBJECTS = \
"CMakeFiles/provider.dir/friendservice.cc.o" \
"CMakeFiles/provider.dir/__/friends.pb.cc.o"

# External object files for target provider
provider_EXTERNAL_OBJECTS =

bin/provider: rpc/callee/CMakeFiles/provider.dir/friendservice.cc.o
bin/provider: rpc/callee/CMakeFiles/provider.dir/__/friends.pb.cc.o
bin/provider: rpc/callee/CMakeFiles/provider.dir/build.make
bin/provider: lib/libmprpc.a
bin/provider: rpc/callee/CMakeFiles/provider.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/qinxin/m_rpc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../bin/provider"
	cd /home/qinxin/m_rpc/rpc/callee && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/provider.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
rpc/callee/CMakeFiles/provider.dir/build: bin/provider
.PHONY : rpc/callee/CMakeFiles/provider.dir/build

rpc/callee/CMakeFiles/provider.dir/clean:
	cd /home/qinxin/m_rpc/rpc/callee && $(CMAKE_COMMAND) -P CMakeFiles/provider.dir/cmake_clean.cmake
.PHONY : rpc/callee/CMakeFiles/provider.dir/clean

rpc/callee/CMakeFiles/provider.dir/depend:
	cd /home/qinxin/m_rpc && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qinxin/m_rpc /home/qinxin/m_rpc/rpc/callee /home/qinxin/m_rpc /home/qinxin/m_rpc/rpc/callee /home/qinxin/m_rpc/rpc/callee/CMakeFiles/provider.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : rpc/callee/CMakeFiles/provider.dir/depend


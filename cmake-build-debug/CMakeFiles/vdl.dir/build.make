# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/patrickli/Desktop/cproject/vdl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/patrickli/Desktop/cproject/vdl/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/vdl.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/vdl.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/vdl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vdl.dir/flags.make

CMakeFiles/vdl.dir/main.c.o: CMakeFiles/vdl.dir/flags.make
CMakeFiles/vdl.dir/main.c.o: ../main.c
CMakeFiles/vdl.dir/main.c.o: CMakeFiles/vdl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/patrickli/Desktop/cproject/vdl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/vdl.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/vdl.dir/main.c.o -MF CMakeFiles/vdl.dir/main.c.o.d -o CMakeFiles/vdl.dir/main.c.o -c /Users/patrickli/Desktop/cproject/vdl/main.c

CMakeFiles/vdl.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vdl.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/patrickli/Desktop/cproject/vdl/main.c > CMakeFiles/vdl.dir/main.c.i

CMakeFiles/vdl.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vdl.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/patrickli/Desktop/cproject/vdl/main.c -o CMakeFiles/vdl.dir/main.c.s

# Object files for target vdl
vdl_OBJECTS = \
"CMakeFiles/vdl.dir/main.c.o"

# External object files for target vdl
vdl_EXTERNAL_OBJECTS =

vdl: CMakeFiles/vdl.dir/main.c.o
vdl: CMakeFiles/vdl.dir/build.make
vdl: CMakeFiles/vdl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/patrickli/Desktop/cproject/vdl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable vdl"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vdl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vdl.dir/build: vdl
.PHONY : CMakeFiles/vdl.dir/build

CMakeFiles/vdl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vdl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vdl.dir/clean

CMakeFiles/vdl.dir/depend:
	cd /Users/patrickli/Desktop/cproject/vdl/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/patrickli/Desktop/cproject/vdl /Users/patrickli/Desktop/cproject/vdl /Users/patrickli/Desktop/cproject/vdl/cmake-build-debug /Users/patrickli/Desktop/cproject/vdl/cmake-build-debug /Users/patrickli/Desktop/cproject/vdl/cmake-build-debug/CMakeFiles/vdl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vdl.dir/depend

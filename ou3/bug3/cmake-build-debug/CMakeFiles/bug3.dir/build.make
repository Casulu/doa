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
CMAKE_COMMAND = /snap/clion/100/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/100/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rasmus/Documents/doa/ou3/bug3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rasmus/Documents/doa/ou3/bug3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/bug3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bug3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bug3.dir/flags.make

CMakeFiles/bug3.dir/solved3.c.o: CMakeFiles/bug3.dir/flags.make
CMakeFiles/bug3.dir/solved3.c.o: ../solved3.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rasmus/Documents/doa/ou3/bug3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/bug3.dir/solved3.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bug3.dir/solved3.c.o   -c /home/rasmus/Documents/doa/ou3/bug3/solved3.c

CMakeFiles/bug3.dir/solved3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bug3.dir/solved3.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rasmus/Documents/doa/ou3/bug3/solved3.c > CMakeFiles/bug3.dir/solved3.c.i

CMakeFiles/bug3.dir/solved3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bug3.dir/solved3.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rasmus/Documents/doa/ou3/bug3/solved3.c -o CMakeFiles/bug3.dir/solved3.c.s

# Object files for target bug3
bug3_OBJECTS = \
"CMakeFiles/bug3.dir/solved3.c.o"

# External object files for target bug3
bug3_EXTERNAL_OBJECTS =

bug3: CMakeFiles/bug3.dir/solved3.c.o
bug3: CMakeFiles/bug3.dir/build.make
bug3: CMakeFiles/bug3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rasmus/Documents/doa/ou3/bug3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bug3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bug3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bug3.dir/build: bug3

.PHONY : CMakeFiles/bug3.dir/build

CMakeFiles/bug3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bug3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bug3.dir/clean

CMakeFiles/bug3.dir/depend:
	cd /home/rasmus/Documents/doa/ou3/bug3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rasmus/Documents/doa/ou3/bug3 /home/rasmus/Documents/doa/ou3/bug3 /home/rasmus/Documents/doa/ou3/bug3/cmake-build-debug /home/rasmus/Documents/doa/ou3/bug3/cmake-build-debug /home/rasmus/Documents/doa/ou3/bug3/cmake-build-debug/CMakeFiles/bug3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bug3.dir/depend


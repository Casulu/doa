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
CMAKE_SOURCE_DIR = /home/rasmus/Documents/doa/ou3/bug4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rasmus/Documents/doa/ou3/bug4/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/bug4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bug4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bug4.dir/flags.make

CMakeFiles/bug4.dir/solved4.c.o: CMakeFiles/bug4.dir/flags.make
CMakeFiles/bug4.dir/solved4.c.o: ../solved4.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rasmus/Documents/doa/ou3/bug4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/bug4.dir/solved4.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bug4.dir/solved4.c.o   -c /home/rasmus/Documents/doa/ou3/bug4/solved4.c

CMakeFiles/bug4.dir/solved4.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bug4.dir/solved4.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rasmus/Documents/doa/ou3/bug4/solved4.c > CMakeFiles/bug4.dir/solved4.c.i

CMakeFiles/bug4.dir/solved4.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bug4.dir/solved4.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rasmus/Documents/doa/ou3/bug4/solved4.c -o CMakeFiles/bug4.dir/solved4.c.s

# Object files for target bug4
bug4_OBJECTS = \
"CMakeFiles/bug4.dir/solved4.c.o"

# External object files for target bug4
bug4_EXTERNAL_OBJECTS =

bug4: CMakeFiles/bug4.dir/solved4.c.o
bug4: CMakeFiles/bug4.dir/build.make
bug4: CMakeFiles/bug4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rasmus/Documents/doa/ou3/bug4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bug4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bug4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bug4.dir/build: bug4

.PHONY : CMakeFiles/bug4.dir/build

CMakeFiles/bug4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bug4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bug4.dir/clean

CMakeFiles/bug4.dir/depend:
	cd /home/rasmus/Documents/doa/ou3/bug4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rasmus/Documents/doa/ou3/bug4 /home/rasmus/Documents/doa/ou3/bug4 /home/rasmus/Documents/doa/ou3/bug4/cmake-build-debug /home/rasmus/Documents/doa/ou3/bug4/cmake-build-debug /home/rasmus/Documents/doa/ou3/bug4/cmake-build-debug/CMakeFiles/bug4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bug4.dir/depend

# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/maksim/work/dd_NBodies

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/maksim/work/dd_NBodies/cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/NBodies.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/NBodies.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/NBodies.dir/flags.make

CMakeFiles/NBodies.dir/main.cpp.o: CMakeFiles/NBodies.dir/flags.make
CMakeFiles/NBodies.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/maksim/work/dd_NBodies/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/NBodies.dir/main.cpp.o"
	/usr/local/bin/g++-10  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/NBodies.dir/main.cpp.o -c /Users/maksim/work/dd_NBodies/main.cpp

CMakeFiles/NBodies.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NBodies.dir/main.cpp.i"
	/usr/local/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/maksim/work/dd_NBodies/main.cpp > CMakeFiles/NBodies.dir/main.cpp.i

CMakeFiles/NBodies.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NBodies.dir/main.cpp.s"
	/usr/local/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/maksim/work/dd_NBodies/main.cpp -o CMakeFiles/NBodies.dir/main.cpp.s

# Object files for target NBodies
NBodies_OBJECTS = \
"CMakeFiles/NBodies.dir/main.cpp.o"

# External object files for target NBodies
NBodies_EXTERNAL_OBJECTS =

NBodies: CMakeFiles/NBodies.dir/main.cpp.o
NBodies: CMakeFiles/NBodies.dir/build.make
NBodies: CMakeFiles/NBodies.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/maksim/work/dd_NBodies/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable NBodies"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NBodies.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/NBodies.dir/build: NBodies

.PHONY : CMakeFiles/NBodies.dir/build

CMakeFiles/NBodies.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NBodies.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NBodies.dir/clean

CMakeFiles/NBodies.dir/depend:
	cd /Users/maksim/work/dd_NBodies/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/maksim/work/dd_NBodies /Users/maksim/work/dd_NBodies /Users/maksim/work/dd_NBodies/cmake-build-release /Users/maksim/work/dd_NBodies/cmake-build-release /Users/maksim/work/dd_NBodies/cmake-build-release/CMakeFiles/NBodies.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NBodies.dir/depend

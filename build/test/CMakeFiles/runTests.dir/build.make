# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/workspace/LOG-Means

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/LOG-Means/build

# Include any dependencies generated for this target.
include test/CMakeFiles/runTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/runTests.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/runTests.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/runTests.dir/flags.make

test/CMakeFiles/runTests.dir/main_test.cpp.o: test/CMakeFiles/runTests.dir/flags.make
test/CMakeFiles/runTests.dir/main_test.cpp.o: ../test/main_test.cpp
test/CMakeFiles/runTests.dir/main_test.cpp.o: test/CMakeFiles/runTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/LOG-Means/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/runTests.dir/main_test.cpp.o"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/runTests.dir/main_test.cpp.o -MF CMakeFiles/runTests.dir/main_test.cpp.o.d -o CMakeFiles/runTests.dir/main_test.cpp.o -c /root/workspace/LOG-Means/test/main_test.cpp

test/CMakeFiles/runTests.dir/main_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/runTests.dir/main_test.cpp.i"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/LOG-Means/test/main_test.cpp > CMakeFiles/runTests.dir/main_test.cpp.i

test/CMakeFiles/runTests.dir/main_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/runTests.dir/main_test.cpp.s"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/LOG-Means/test/main_test.cpp -o CMakeFiles/runTests.dir/main_test.cpp.s

test/CMakeFiles/runTests.dir/data_preprocessing_test.cpp.o: test/CMakeFiles/runTests.dir/flags.make
test/CMakeFiles/runTests.dir/data_preprocessing_test.cpp.o: ../test/data_preprocessing_test.cpp
test/CMakeFiles/runTests.dir/data_preprocessing_test.cpp.o: test/CMakeFiles/runTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/LOG-Means/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/runTests.dir/data_preprocessing_test.cpp.o"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/runTests.dir/data_preprocessing_test.cpp.o -MF CMakeFiles/runTests.dir/data_preprocessing_test.cpp.o.d -o CMakeFiles/runTests.dir/data_preprocessing_test.cpp.o -c /root/workspace/LOG-Means/test/data_preprocessing_test.cpp

test/CMakeFiles/runTests.dir/data_preprocessing_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/runTests.dir/data_preprocessing_test.cpp.i"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/LOG-Means/test/data_preprocessing_test.cpp > CMakeFiles/runTests.dir/data_preprocessing_test.cpp.i

test/CMakeFiles/runTests.dir/data_preprocessing_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/runTests.dir/data_preprocessing_test.cpp.s"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/LOG-Means/test/data_preprocessing_test.cpp -o CMakeFiles/runTests.dir/data_preprocessing_test.cpp.s

test/CMakeFiles/runTests.dir/gain_calculation_test.cpp.o: test/CMakeFiles/runTests.dir/flags.make
test/CMakeFiles/runTests.dir/gain_calculation_test.cpp.o: ../test/gain_calculation_test.cpp
test/CMakeFiles/runTests.dir/gain_calculation_test.cpp.o: test/CMakeFiles/runTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/LOG-Means/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object test/CMakeFiles/runTests.dir/gain_calculation_test.cpp.o"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/runTests.dir/gain_calculation_test.cpp.o -MF CMakeFiles/runTests.dir/gain_calculation_test.cpp.o.d -o CMakeFiles/runTests.dir/gain_calculation_test.cpp.o -c /root/workspace/LOG-Means/test/gain_calculation_test.cpp

test/CMakeFiles/runTests.dir/gain_calculation_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/runTests.dir/gain_calculation_test.cpp.i"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/LOG-Means/test/gain_calculation_test.cpp > CMakeFiles/runTests.dir/gain_calculation_test.cpp.i

test/CMakeFiles/runTests.dir/gain_calculation_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/runTests.dir/gain_calculation_test.cpp.s"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/LOG-Means/test/gain_calculation_test.cpp -o CMakeFiles/runTests.dir/gain_calculation_test.cpp.s

test/CMakeFiles/runTests.dir/cluster_analysis_test.cpp.o: test/CMakeFiles/runTests.dir/flags.make
test/CMakeFiles/runTests.dir/cluster_analysis_test.cpp.o: ../test/cluster_analysis_test.cpp
test/CMakeFiles/runTests.dir/cluster_analysis_test.cpp.o: test/CMakeFiles/runTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/LOG-Means/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object test/CMakeFiles/runTests.dir/cluster_analysis_test.cpp.o"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/runTests.dir/cluster_analysis_test.cpp.o -MF CMakeFiles/runTests.dir/cluster_analysis_test.cpp.o.d -o CMakeFiles/runTests.dir/cluster_analysis_test.cpp.o -c /root/workspace/LOG-Means/test/cluster_analysis_test.cpp

test/CMakeFiles/runTests.dir/cluster_analysis_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/runTests.dir/cluster_analysis_test.cpp.i"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/LOG-Means/test/cluster_analysis_test.cpp > CMakeFiles/runTests.dir/cluster_analysis_test.cpp.i

test/CMakeFiles/runTests.dir/cluster_analysis_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/runTests.dir/cluster_analysis_test.cpp.s"
	cd /root/workspace/LOG-Means/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/LOG-Means/test/cluster_analysis_test.cpp -o CMakeFiles/runTests.dir/cluster_analysis_test.cpp.s

# Object files for target runTests
runTests_OBJECTS = \
"CMakeFiles/runTests.dir/main_test.cpp.o" \
"CMakeFiles/runTests.dir/data_preprocessing_test.cpp.o" \
"CMakeFiles/runTests.dir/gain_calculation_test.cpp.o" \
"CMakeFiles/runTests.dir/cluster_analysis_test.cpp.o"

# External object files for target runTests
runTests_EXTERNAL_OBJECTS =

test/runTests: test/CMakeFiles/runTests.dir/main_test.cpp.o
test/runTests: test/CMakeFiles/runTests.dir/data_preprocessing_test.cpp.o
test/runTests: test/CMakeFiles/runTests.dir/gain_calculation_test.cpp.o
test/runTests: test/CMakeFiles/runTests.dir/cluster_analysis_test.cpp.o
test/runTests: test/CMakeFiles/runTests.dir/build.make
test/runTests: /usr/local/lib/libgtest.a
test/runTests: /usr/local/lib/libgtest_main.a
test/runTests: /usr/local/lib/libgtest_main.a
test/runTests: /usr/local/lib/libgtest.a
test/runTests: test/CMakeFiles/runTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/LOG-Means/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable runTests"
	cd /root/workspace/LOG-Means/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/runTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/runTests.dir/build: test/runTests
.PHONY : test/CMakeFiles/runTests.dir/build

test/CMakeFiles/runTests.dir/clean:
	cd /root/workspace/LOG-Means/build/test && $(CMAKE_COMMAND) -P CMakeFiles/runTests.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/runTests.dir/clean

test/CMakeFiles/runTests.dir/depend:
	cd /root/workspace/LOG-Means/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/LOG-Means /root/workspace/LOG-Means/test /root/workspace/LOG-Means/build /root/workspace/LOG-Means/build/test /root/workspace/LOG-Means/build/test/CMakeFiles/runTests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/runTests.dir/depend


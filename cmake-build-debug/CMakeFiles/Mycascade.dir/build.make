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
CMAKE_COMMAND = /snap/clion/209/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/209/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/marco/Desktop/Thesis

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marco/Desktop/Thesis/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Mycascade.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Mycascade.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Mycascade.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Mycascade.dir/flags.make

CMakeFiles/Mycascade.dir/main.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/main.cpp.o: ../main.cpp
CMakeFiles/Mycascade.dir/main.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Mycascade.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/main.cpp.o -MF CMakeFiles/Mycascade.dir/main.cpp.o.d -o CMakeFiles/Mycascade.dir/main.cpp.o -c /home/marco/Desktop/Thesis/main.cpp

CMakeFiles/Mycascade.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/main.cpp > CMakeFiles/Mycascade.dir/main.cpp.i

CMakeFiles/Mycascade.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/main.cpp -o CMakeFiles/Mycascade.dir/main.cpp.s

CMakeFiles/Mycascade.dir/reconciliation.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/reconciliation.cpp.o: ../reconciliation.cpp
CMakeFiles/Mycascade.dir/reconciliation.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Mycascade.dir/reconciliation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/reconciliation.cpp.o -MF CMakeFiles/Mycascade.dir/reconciliation.cpp.o.d -o CMakeFiles/Mycascade.dir/reconciliation.cpp.o -c /home/marco/Desktop/Thesis/reconciliation.cpp

CMakeFiles/Mycascade.dir/reconciliation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/reconciliation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/reconciliation.cpp > CMakeFiles/Mycascade.dir/reconciliation.cpp.i

CMakeFiles/Mycascade.dir/reconciliation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/reconciliation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/reconciliation.cpp -o CMakeFiles/Mycascade.dir/reconciliation.cpp.s

CMakeFiles/Mycascade.dir/block.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/block.cpp.o: ../block.cpp
CMakeFiles/Mycascade.dir/block.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Mycascade.dir/block.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/block.cpp.o -MF CMakeFiles/Mycascade.dir/block.cpp.o.d -o CMakeFiles/Mycascade.dir/block.cpp.o -c /home/marco/Desktop/Thesis/block.cpp

CMakeFiles/Mycascade.dir/block.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/block.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/block.cpp > CMakeFiles/Mycascade.dir/block.cpp.i

CMakeFiles/Mycascade.dir/block.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/block.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/block.cpp -o CMakeFiles/Mycascade.dir/block.cpp.s

CMakeFiles/Mycascade.dir/algorithm.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/algorithm.cpp.o: ../algorithm.cpp
CMakeFiles/Mycascade.dir/algorithm.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Mycascade.dir/algorithm.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/algorithm.cpp.o -MF CMakeFiles/Mycascade.dir/algorithm.cpp.o.d -o CMakeFiles/Mycascade.dir/algorithm.cpp.o -c /home/marco/Desktop/Thesis/algorithm.cpp

CMakeFiles/Mycascade.dir/algorithm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/algorithm.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/algorithm.cpp > CMakeFiles/Mycascade.dir/algorithm.cpp.i

CMakeFiles/Mycascade.dir/algorithm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/algorithm.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/algorithm.cpp -o CMakeFiles/Mycascade.dir/algorithm.cpp.s

CMakeFiles/Mycascade.dir/classical_session.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/classical_session.cpp.o: ../classical_session.cpp
CMakeFiles/Mycascade.dir/classical_session.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Mycascade.dir/classical_session.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/classical_session.cpp.o -MF CMakeFiles/Mycascade.dir/classical_session.cpp.o.d -o CMakeFiles/Mycascade.dir/classical_session.cpp.o -c /home/marco/Desktop/Thesis/classical_session.cpp

CMakeFiles/Mycascade.dir/classical_session.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/classical_session.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/classical_session.cpp > CMakeFiles/Mycascade.dir/classical_session.cpp.i

CMakeFiles/Mycascade.dir/classical_session.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/classical_session.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/classical_session.cpp -o CMakeFiles/Mycascade.dir/classical_session.cpp.s

CMakeFiles/Mycascade.dir/debug.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/debug.cpp.o: ../debug.cpp
CMakeFiles/Mycascade.dir/debug.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Mycascade.dir/debug.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/debug.cpp.o -MF CMakeFiles/Mycascade.dir/debug.cpp.o.d -o CMakeFiles/Mycascade.dir/debug.cpp.o -c /home/marco/Desktop/Thesis/debug.cpp

CMakeFiles/Mycascade.dir/debug.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/debug.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/debug.cpp > CMakeFiles/Mycascade.dir/debug.cpp.i

CMakeFiles/Mycascade.dir/debug.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/debug.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/debug.cpp -o CMakeFiles/Mycascade.dir/debug.cpp.s

CMakeFiles/Mycascade.dir/iteration.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/iteration.cpp.o: ../iteration.cpp
CMakeFiles/Mycascade.dir/iteration.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Mycascade.dir/iteration.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/iteration.cpp.o -MF CMakeFiles/Mycascade.dir/iteration.cpp.o.d -o CMakeFiles/Mycascade.dir/iteration.cpp.o -c /home/marco/Desktop/Thesis/iteration.cpp

CMakeFiles/Mycascade.dir/iteration.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/iteration.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/iteration.cpp > CMakeFiles/Mycascade.dir/iteration.cpp.i

CMakeFiles/Mycascade.dir/iteration.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/iteration.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/iteration.cpp -o CMakeFiles/Mycascade.dir/iteration.cpp.s

CMakeFiles/Mycascade.dir/key.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/key.cpp.o: ../key.cpp
CMakeFiles/Mycascade.dir/key.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Mycascade.dir/key.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/key.cpp.o -MF CMakeFiles/Mycascade.dir/key.cpp.o.d -o CMakeFiles/Mycascade.dir/key.cpp.o -c /home/marco/Desktop/Thesis/key.cpp

CMakeFiles/Mycascade.dir/key.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/key.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/key.cpp > CMakeFiles/Mycascade.dir/key.cpp.i

CMakeFiles/Mycascade.dir/key.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/key.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/key.cpp -o CMakeFiles/Mycascade.dir/key.cpp.s

CMakeFiles/Mycascade.dir/mock_classical_session.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/mock_classical_session.cpp.o: ../mock_classical_session.cpp
CMakeFiles/Mycascade.dir/mock_classical_session.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Mycascade.dir/mock_classical_session.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/mock_classical_session.cpp.o -MF CMakeFiles/Mycascade.dir/mock_classical_session.cpp.o.d -o CMakeFiles/Mycascade.dir/mock_classical_session.cpp.o -c /home/marco/Desktop/Thesis/mock_classical_session.cpp

CMakeFiles/Mycascade.dir/mock_classical_session.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/mock_classical_session.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/mock_classical_session.cpp > CMakeFiles/Mycascade.dir/mock_classical_session.cpp.i

CMakeFiles/Mycascade.dir/mock_classical_session.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/mock_classical_session.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/mock_classical_session.cpp -o CMakeFiles/Mycascade.dir/mock_classical_session.cpp.s

CMakeFiles/Mycascade.dir/pending_item.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/pending_item.cpp.o: ../pending_item.cpp
CMakeFiles/Mycascade.dir/pending_item.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Mycascade.dir/pending_item.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/pending_item.cpp.o -MF CMakeFiles/Mycascade.dir/pending_item.cpp.o.d -o CMakeFiles/Mycascade.dir/pending_item.cpp.o -c /home/marco/Desktop/Thesis/pending_item.cpp

CMakeFiles/Mycascade.dir/pending_item.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/pending_item.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/pending_item.cpp > CMakeFiles/Mycascade.dir/pending_item.cpp.i

CMakeFiles/Mycascade.dir/pending_item.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/pending_item.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/pending_item.cpp -o CMakeFiles/Mycascade.dir/pending_item.cpp.s

CMakeFiles/Mycascade.dir/random.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/random.cpp.o: ../random.cpp
CMakeFiles/Mycascade.dir/random.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Mycascade.dir/random.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/random.cpp.o -MF CMakeFiles/Mycascade.dir/random.cpp.o.d -o CMakeFiles/Mycascade.dir/random.cpp.o -c /home/marco/Desktop/Thesis/random.cpp

CMakeFiles/Mycascade.dir/random.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/random.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/random.cpp > CMakeFiles/Mycascade.dir/random.cpp.i

CMakeFiles/Mycascade.dir/random.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/random.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/random.cpp -o CMakeFiles/Mycascade.dir/random.cpp.s

CMakeFiles/Mycascade.dir/report.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/report.cpp.o: ../report.cpp
CMakeFiles/Mycascade.dir/report.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Mycascade.dir/report.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/report.cpp.o -MF CMakeFiles/Mycascade.dir/report.cpp.o.d -o CMakeFiles/Mycascade.dir/report.cpp.o -c /home/marco/Desktop/Thesis/report.cpp

CMakeFiles/Mycascade.dir/report.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/report.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/report.cpp > CMakeFiles/Mycascade.dir/report.cpp.i

CMakeFiles/Mycascade.dir/report.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/report.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/report.cpp -o CMakeFiles/Mycascade.dir/report.cpp.s

CMakeFiles/Mycascade.dir/shuffle.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/shuffle.cpp.o: ../shuffle.cpp
CMakeFiles/Mycascade.dir/shuffle.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/Mycascade.dir/shuffle.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/shuffle.cpp.o -MF CMakeFiles/Mycascade.dir/shuffle.cpp.o.d -o CMakeFiles/Mycascade.dir/shuffle.cpp.o -c /home/marco/Desktop/Thesis/shuffle.cpp

CMakeFiles/Mycascade.dir/shuffle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/shuffle.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/shuffle.cpp > CMakeFiles/Mycascade.dir/shuffle.cpp.i

CMakeFiles/Mycascade.dir/shuffle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/shuffle.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/shuffle.cpp -o CMakeFiles/Mycascade.dir/shuffle.cpp.s

CMakeFiles/Mycascade.dir/shuffled_key.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/shuffled_key.cpp.o: ../shuffled_key.cpp
CMakeFiles/Mycascade.dir/shuffled_key.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/Mycascade.dir/shuffled_key.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/shuffled_key.cpp.o -MF CMakeFiles/Mycascade.dir/shuffled_key.cpp.o.d -o CMakeFiles/Mycascade.dir/shuffled_key.cpp.o -c /home/marco/Desktop/Thesis/shuffled_key.cpp

CMakeFiles/Mycascade.dir/shuffled_key.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/shuffled_key.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/shuffled_key.cpp > CMakeFiles/Mycascade.dir/shuffled_key.cpp.i

CMakeFiles/Mycascade.dir/shuffled_key.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/shuffled_key.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/shuffled_key.cpp -o CMakeFiles/Mycascade.dir/shuffled_key.cpp.s

CMakeFiles/Mycascade.dir/stats.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/stats.cpp.o: ../stats.cpp
CMakeFiles/Mycascade.dir/stats.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/Mycascade.dir/stats.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/stats.cpp.o -MF CMakeFiles/Mycascade.dir/stats.cpp.o.d -o CMakeFiles/Mycascade.dir/stats.cpp.o -c /home/marco/Desktop/Thesis/stats.cpp

CMakeFiles/Mycascade.dir/stats.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/stats.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/stats.cpp > CMakeFiles/Mycascade.dir/stats.cpp.i

CMakeFiles/Mycascade.dir/stats.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/stats.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/stats.cpp -o CMakeFiles/Mycascade.dir/stats.cpp.s

CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.o: CMakeFiles/Mycascade.dir/flags.make
CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.o: ../RabbitmqClassicalSession.cpp
CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.o: CMakeFiles/Mycascade.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.o -MF CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.o.d -o CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.o -c /home/marco/Desktop/Thesis/RabbitmqClassicalSession.cpp

CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/Desktop/Thesis/RabbitmqClassicalSession.cpp > CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.i

CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/Desktop/Thesis/RabbitmqClassicalSession.cpp -o CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.s

# Object files for target Mycascade
Mycascade_OBJECTS = \
"CMakeFiles/Mycascade.dir/main.cpp.o" \
"CMakeFiles/Mycascade.dir/reconciliation.cpp.o" \
"CMakeFiles/Mycascade.dir/block.cpp.o" \
"CMakeFiles/Mycascade.dir/algorithm.cpp.o" \
"CMakeFiles/Mycascade.dir/classical_session.cpp.o" \
"CMakeFiles/Mycascade.dir/debug.cpp.o" \
"CMakeFiles/Mycascade.dir/iteration.cpp.o" \
"CMakeFiles/Mycascade.dir/key.cpp.o" \
"CMakeFiles/Mycascade.dir/mock_classical_session.cpp.o" \
"CMakeFiles/Mycascade.dir/pending_item.cpp.o" \
"CMakeFiles/Mycascade.dir/random.cpp.o" \
"CMakeFiles/Mycascade.dir/report.cpp.o" \
"CMakeFiles/Mycascade.dir/shuffle.cpp.o" \
"CMakeFiles/Mycascade.dir/shuffled_key.cpp.o" \
"CMakeFiles/Mycascade.dir/stats.cpp.o" \
"CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.o"

# External object files for target Mycascade
Mycascade_EXTERNAL_OBJECTS =

Mycascade: CMakeFiles/Mycascade.dir/main.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/reconciliation.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/block.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/algorithm.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/classical_session.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/debug.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/iteration.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/key.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/mock_classical_session.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/pending_item.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/random.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/report.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/shuffle.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/shuffled_key.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/stats.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/RabbitmqClassicalSession.cpp.o
Mycascade: CMakeFiles/Mycascade.dir/build.make
Mycascade: CMakeFiles/Mycascade.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Linking CXX executable Mycascade"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Mycascade.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Mycascade.dir/build: Mycascade
.PHONY : CMakeFiles/Mycascade.dir/build

CMakeFiles/Mycascade.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Mycascade.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Mycascade.dir/clean

CMakeFiles/Mycascade.dir/depend:
	cd /home/marco/Desktop/Thesis/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marco/Desktop/Thesis /home/marco/Desktop/Thesis /home/marco/Desktop/Thesis/cmake-build-debug /home/marco/Desktop/Thesis/cmake-build-debug /home/marco/Desktop/Thesis/cmake-build-debug/CMakeFiles/Mycascade.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Mycascade.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /home/wang/clion/clion-2018.1.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/wang/clion/clion-2018.1.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wang/llvm-3.5.2/llvm/llvm-sol

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/llvm_sol.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/llvm_sol.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/llvm_sol.dir/flags.make

CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o: CMakeFiles/llvm_sol.dir/flags.make
CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o: ../Json/SolcJsonOperator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o -c /home/wang/llvm-3.5.2/llvm/llvm-sol/Json/SolcJsonOperator.cpp

CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wang/llvm-3.5.2/llvm/llvm-sol/Json/SolcJsonOperator.cpp > CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.i

CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wang/llvm-3.5.2/llvm/llvm-sol/Json/SolcJsonOperator.cpp -o CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.s

CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o.requires:

.PHONY : CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o.requires

CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o.provides: CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o.requires
	$(MAKE) -f CMakeFiles/llvm_sol.dir/build.make CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o.provides.build
.PHONY : CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o.provides

CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o.provides.build: CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o


CMakeFiles/llvm_sol.dir/SolcMain.cpp.o: CMakeFiles/llvm_sol.dir/flags.make
CMakeFiles/llvm_sol.dir/SolcMain.cpp.o: ../SolcMain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/llvm_sol.dir/SolcMain.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/llvm_sol.dir/SolcMain.cpp.o -c /home/wang/llvm-3.5.2/llvm/llvm-sol/SolcMain.cpp

CMakeFiles/llvm_sol.dir/SolcMain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/llvm_sol.dir/SolcMain.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wang/llvm-3.5.2/llvm/llvm-sol/SolcMain.cpp > CMakeFiles/llvm_sol.dir/SolcMain.cpp.i

CMakeFiles/llvm_sol.dir/SolcMain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/llvm_sol.dir/SolcMain.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wang/llvm-3.5.2/llvm/llvm-sol/SolcMain.cpp -o CMakeFiles/llvm_sol.dir/SolcMain.cpp.s

CMakeFiles/llvm_sol.dir/SolcMain.cpp.o.requires:

.PHONY : CMakeFiles/llvm_sol.dir/SolcMain.cpp.o.requires

CMakeFiles/llvm_sol.dir/SolcMain.cpp.o.provides: CMakeFiles/llvm_sol.dir/SolcMain.cpp.o.requires
	$(MAKE) -f CMakeFiles/llvm_sol.dir/build.make CMakeFiles/llvm_sol.dir/SolcMain.cpp.o.provides.build
.PHONY : CMakeFiles/llvm_sol.dir/SolcMain.cpp.o.provides

CMakeFiles/llvm_sol.dir/SolcMain.cpp.o.provides.build: CMakeFiles/llvm_sol.dir/SolcMain.cpp.o


CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o: CMakeFiles/llvm_sol.dir/flags.make
CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o: ../Json/jsoncpp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o -c /home/wang/llvm-3.5.2/llvm/llvm-sol/Json/jsoncpp.cpp

CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wang/llvm-3.5.2/llvm/llvm-sol/Json/jsoncpp.cpp > CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.i

CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wang/llvm-3.5.2/llvm/llvm-sol/Json/jsoncpp.cpp -o CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.s

CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o.requires:

.PHONY : CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o.requires

CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o.provides: CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o.requires
	$(MAKE) -f CMakeFiles/llvm_sol.dir/build.make CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o.provides.build
.PHONY : CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o.provides

CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o.provides.build: CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o


CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o: CMakeFiles/llvm_sol.dir/flags.make
CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o: ../Builder/SolcLLVM.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o -c /home/wang/llvm-3.5.2/llvm/llvm-sol/Builder/SolcLLVM.cpp

CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wang/llvm-3.5.2/llvm/llvm-sol/Builder/SolcLLVM.cpp > CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.i

CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wang/llvm-3.5.2/llvm/llvm-sol/Builder/SolcLLVM.cpp -o CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.s

CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o.requires:

.PHONY : CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o.requires

CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o.provides: CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o.requires
	$(MAKE) -f CMakeFiles/llvm_sol.dir/build.make CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o.provides.build
.PHONY : CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o.provides

CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o.provides.build: CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o


# Object files for target llvm_sol
llvm_sol_OBJECTS = \
"CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o" \
"CMakeFiles/llvm_sol.dir/SolcMain.cpp.o" \
"CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o" \
"CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o"

# External object files for target llvm_sol
llvm_sol_EXTERNAL_OBJECTS =

llvm_sol: CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o
llvm_sol: CMakeFiles/llvm_sol.dir/SolcMain.cpp.o
llvm_sol: CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o
llvm_sol: CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o
llvm_sol: CMakeFiles/llvm_sol.dir/build.make
llvm_sol: CMakeFiles/llvm_sol.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable llvm_sol"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/llvm_sol.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/llvm_sol.dir/build: llvm_sol

.PHONY : CMakeFiles/llvm_sol.dir/build

CMakeFiles/llvm_sol.dir/requires: CMakeFiles/llvm_sol.dir/Json/SolcJsonOperator.cpp.o.requires
CMakeFiles/llvm_sol.dir/requires: CMakeFiles/llvm_sol.dir/SolcMain.cpp.o.requires
CMakeFiles/llvm_sol.dir/requires: CMakeFiles/llvm_sol.dir/Json/jsoncpp.cpp.o.requires
CMakeFiles/llvm_sol.dir/requires: CMakeFiles/llvm_sol.dir/Builder/SolcLLVM.cpp.o.requires

.PHONY : CMakeFiles/llvm_sol.dir/requires

CMakeFiles/llvm_sol.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/llvm_sol.dir/cmake_clean.cmake
.PHONY : CMakeFiles/llvm_sol.dir/clean

CMakeFiles/llvm_sol.dir/depend:
	cd /home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wang/llvm-3.5.2/llvm/llvm-sol /home/wang/llvm-3.5.2/llvm/llvm-sol /home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug /home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug /home/wang/llvm-3.5.2/llvm/llvm-sol/cmake-build-debug/CMakeFiles/llvm_sol.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/llvm_sol.dir/depend


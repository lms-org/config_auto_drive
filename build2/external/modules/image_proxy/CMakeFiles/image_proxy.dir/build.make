# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.3.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.3.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lukas/phoenix_lms/config_auto_drive

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lukas/phoenix_lms/config_auto_drive/build2

# Include any dependencies generated for this target.
include external/modules/image_proxy/CMakeFiles/image_proxy.dir/depend.make

# Include the progress variables for this target.
include external/modules/image_proxy/CMakeFiles/image_proxy.dir/progress.make

# Include the compile flags for this target's objects.
include external/modules/image_proxy/CMakeFiles/image_proxy.dir/flags.make

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o: external/modules/image_proxy/CMakeFiles/image_proxy.dir/flags.make
external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o: ../external/modules/image_proxy/src/interface.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukas/phoenix_lms/config_auto_drive/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/image_proxy.dir/src/interface.cpp.o -c /Users/lukas/phoenix_lms/config_auto_drive/external/modules/image_proxy/src/interface.cpp

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/image_proxy.dir/src/interface.cpp.i"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/lukas/phoenix_lms/config_auto_drive/external/modules/image_proxy/src/interface.cpp > CMakeFiles/image_proxy.dir/src/interface.cpp.i

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/image_proxy.dir/src/interface.cpp.s"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/lukas/phoenix_lms/config_auto_drive/external/modules/image_proxy/src/interface.cpp -o CMakeFiles/image_proxy.dir/src/interface.cpp.s

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o.requires:

.PHONY : external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o.requires

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o.provides: external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o.requires
	$(MAKE) -f external/modules/image_proxy/CMakeFiles/image_proxy.dir/build.make external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o.provides.build
.PHONY : external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o.provides

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o.provides.build: external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o


external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o: external/modules/image_proxy/CMakeFiles/image_proxy.dir/flags.make
external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o: ../external/modules/image_proxy/src/image_proxy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukas/phoenix_lms/config_auto_drive/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o -c /Users/lukas/phoenix_lms/config_auto_drive/external/modules/image_proxy/src/image_proxy.cpp

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/image_proxy.dir/src/image_proxy.cpp.i"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/lukas/phoenix_lms/config_auto_drive/external/modules/image_proxy/src/image_proxy.cpp > CMakeFiles/image_proxy.dir/src/image_proxy.cpp.i

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/image_proxy.dir/src/image_proxy.cpp.s"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/lukas/phoenix_lms/config_auto_drive/external/modules/image_proxy/src/image_proxy.cpp -o CMakeFiles/image_proxy.dir/src/image_proxy.cpp.s

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o.requires:

.PHONY : external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o.requires

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o.provides: external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o.requires
	$(MAKE) -f external/modules/image_proxy/CMakeFiles/image_proxy.dir/build.make external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o.provides.build
.PHONY : external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o.provides

external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o.provides.build: external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o


# Object files for target image_proxy
image_proxy_OBJECTS = \
"CMakeFiles/image_proxy.dir/src/interface.cpp.o" \
"CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o"

# External object files for target image_proxy
image_proxy_EXTERNAL_OBJECTS =

external/modules/libimage_proxy.so: external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o
external/modules/libimage_proxy.so: external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o
external/modules/libimage_proxy.so: external/modules/image_proxy/CMakeFiles/image_proxy.dir/build.make
external/modules/libimage_proxy.so: external/libraries/imaging/libimaging.dylib
external/modules/libimage_proxy.so: LMS/lms/liblmscore.dylib
external/modules/libimage_proxy.so: external/modules/image_proxy/CMakeFiles/image_proxy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/lukas/phoenix_lms/config_auto_drive/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared module ../libimage_proxy.so"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/image_proxy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/modules/image_proxy/CMakeFiles/image_proxy.dir/build: external/modules/libimage_proxy.so

.PHONY : external/modules/image_proxy/CMakeFiles/image_proxy.dir/build

external/modules/image_proxy/CMakeFiles/image_proxy.dir/requires: external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/interface.cpp.o.requires
external/modules/image_proxy/CMakeFiles/image_proxy.dir/requires: external/modules/image_proxy/CMakeFiles/image_proxy.dir/src/image_proxy.cpp.o.requires

.PHONY : external/modules/image_proxy/CMakeFiles/image_proxy.dir/requires

external/modules/image_proxy/CMakeFiles/image_proxy.dir/clean:
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy && $(CMAKE_COMMAND) -P CMakeFiles/image_proxy.dir/cmake_clean.cmake
.PHONY : external/modules/image_proxy/CMakeFiles/image_proxy.dir/clean

external/modules/image_proxy/CMakeFiles/image_proxy.dir/depend:
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lukas/phoenix_lms/config_auto_drive /Users/lukas/phoenix_lms/config_auto_drive/external/modules/image_proxy /Users/lukas/phoenix_lms/config_auto_drive/build2 /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/image_proxy/CMakeFiles/image_proxy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/modules/image_proxy/CMakeFiles/image_proxy.dir/depend


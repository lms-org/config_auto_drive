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
include external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/depend.make

# Include the progress variables for this target.
include external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/progress.make

# Include the compile flags for this target's objects.
include external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/flags.make

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/flags.make
external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o: ../external/modules/usb_sense_link/src/usb_sense_link.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukas/phoenix_lms/config_auto_drive/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o -c /Users/lukas/phoenix_lms/config_auto_drive/external/modules/usb_sense_link/src/usb_sense_link.cpp

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.i"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/lukas/phoenix_lms/config_auto_drive/external/modules/usb_sense_link/src/usb_sense_link.cpp > CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.i

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.s"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/lukas/phoenix_lms/config_auto_drive/external/modules/usb_sense_link/src/usb_sense_link.cpp -o CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.s

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o.requires:

.PHONY : external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o.requires

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o.provides: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o.requires
	$(MAKE) -f external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/build.make external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o.provides.build
.PHONY : external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o.provides

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o.provides.build: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o


external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/flags.make
external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o: ../external/modules/usb_sense_link/src/interface.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukas/phoenix_lms/config_auto_drive/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/usb_sense_link.dir/src/interface.cpp.o -c /Users/lukas/phoenix_lms/config_auto_drive/external/modules/usb_sense_link/src/interface.cpp

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/usb_sense_link.dir/src/interface.cpp.i"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/lukas/phoenix_lms/config_auto_drive/external/modules/usb_sense_link/src/interface.cpp > CMakeFiles/usb_sense_link.dir/src/interface.cpp.i

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/usb_sense_link.dir/src/interface.cpp.s"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/lukas/phoenix_lms/config_auto_drive/external/modules/usb_sense_link/src/interface.cpp -o CMakeFiles/usb_sense_link.dir/src/interface.cpp.s

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o.requires:

.PHONY : external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o.requires

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o.provides: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o.requires
	$(MAKE) -f external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/build.make external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o.provides.build
.PHONY : external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o.provides

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o.provides.build: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o


# Object files for target usb_sense_link
usb_sense_link_OBJECTS = \
"CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o" \
"CMakeFiles/usb_sense_link.dir/src/interface.cpp.o"

# External object files for target usb_sense_link
usb_sense_link_EXTERNAL_OBJECTS =

external/modules/libusb_sense_link.so: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o
external/modules/libusb_sense_link.so: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o
external/modules/libusb_sense_link.so: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/build.make
external/modules/libusb_sense_link.so: external/libraries/sense_link/libsense_link.dylib
external/modules/libusb_sense_link.so: LMS/lms/liblmscore.dylib
external/modules/libusb_sense_link.so: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/lukas/phoenix_lms/config_auto_drive/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared module ../libusb_sense_link.so"
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/usb_sense_link.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/build: external/modules/libusb_sense_link.so

.PHONY : external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/build

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/requires: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/usb_sense_link.cpp.o.requires
external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/requires: external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/src/interface.cpp.o.requires

.PHONY : external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/requires

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/clean:
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link && $(CMAKE_COMMAND) -P CMakeFiles/usb_sense_link.dir/cmake_clean.cmake
.PHONY : external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/clean

external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/depend:
	cd /Users/lukas/phoenix_lms/config_auto_drive/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lukas/phoenix_lms/config_auto_drive /Users/lukas/phoenix_lms/config_auto_drive/external/modules/usb_sense_link /Users/lukas/phoenix_lms/config_auto_drive/build2 /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link /Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/modules/usb_sense_link/CMakeFiles/usb_sense_link.dir/depend


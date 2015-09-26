cmake_minimum_required(VERSION 2.8)

# This macro is used in external/modules and external/libraries CMakeLists
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

set(LMS_STANDALONE FALSE)
set(LMS_DIR_CONFIGS ${CMAKE_CURRENT_SOURCE_DIR}/configs)
set(LMS_DIR_EXTERNAL ${CMAKE_CURRENT_BINARY_DIR}/external)
set(LMS_DIR_MODULES ${CMAKE_CURRENT_BINARY_DIR}/external/modules)

if(POLICY CMP0042)
    # Set old MACOSX_RPATH policy on OS X
    cmake_policy(SET CMP0042 OLD)
endif()

#set compiler flags
if((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -std=c++0x -Wall -Wextra -pedantic -O0 ")
endif()

# show config files in QtCreator
file(GLOB_RECURSE CONFIG_FILES_LCONF "**/*.lconf")
file(GLOB_RECURSE CONFIG_FILES_XML "**/*.xml")
add_custom_target(show_configs SOURCES ${CONFIG_FILES_LCONF} ${CONFIG_FILES_XML})

#compile core
add_subdirectory(LMS/lms)

#Add shared headers
file(GLOB_RECURSE SHARED_HEADER **/CMakeSharedHeaders.txt)
if (SHARED_HEADER)
        foreach(filepath ${SHARED_HEADER})
                get_filename_component(FOLDER ${filepath} DIRECTORY)
                include_directories(${FOLDER})
                include("${FOLDER}/CMakeSharedHeaders.txt")
	endforeach()
endif()

#compile libs/modules
add_subdirectory(external)

add_subdirectory(LMS/lms/test)

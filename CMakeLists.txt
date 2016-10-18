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

if(POLICY CMP0042)
    # Set old MACOSX_RPATH policy on OS X
    cmake_policy(SET CMP0042 OLD)
endif()

#set compiler flags
if((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wreturn-type -Wpedantic ")
endif()

# show config files in QtCreator
file(GLOB_RECURSE CONFIG_FILES_LCONF "**/*.lconf")
file(GLOB_RECURSE CONFIG_FILES_XML "**/*.xml")
add_custom_target(show_configs SOURCES ${CONFIG_FILES_LCONF} ${CONFIG_FILES_XML})

#Add shared headers
file(GLOB_RECURSE SHARED_HEADER **/CMakeSharedHeaders.txt ${CMAKE_CURRENT_BINARY_DIR}/**/CMakeSharedHeaders.txt)
if (SHARED_HEADER)
        foreach(filepath ${SHARED_HEADER})
                get_filename_component(FOLDER ${filepath} DIRECTORY)
                include_directories(${FOLDER})
                include("${FOLDER}/CMakeSharedHeaders.txt")
	endforeach()
endif()

#Clion bug that doesn't add usr/local/include usr/local/lib
if(APPLE)
include_directories(usr/local/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} usr/local/lib)
endif()

#add conan libs/includes
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_CURRENT_BINARY_DIR}/lib")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")
link_directories(${CMAKE_LIBRARY_PATH}) #TODO why is this needed?

#compile libs/modules
add_subdirectory(external)

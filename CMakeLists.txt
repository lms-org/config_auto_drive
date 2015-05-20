cmake_minimum_required(VERSION 2.8)
#add macro
#TODO put it in another file
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


#set defines
#set external-directory
add_definitions(-DEXTERNAL_DIR="${CMAKE_CURRENT_BINARY_DIR}/external")
#set config-directory
add_definitions(-DCONFIGS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/configs")

#set compiler flags
if((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -std=c++0x -Wall -Wextra -pedantic -O0 ")
endif()

# show config files in QtCreator
file(GLOB_RECURSE CONFIG_FILES_LCONF "**/*.lconf")
file(GLOB_RECURSE CONFIG_FILES_XML "**/*.xml")
add_executable(stub stub.cpp ${CONFIG_FILES_LCONF} ${CONFIG_FILES_XML})

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

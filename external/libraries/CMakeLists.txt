#Build libraries given by CmakeData
#Create CMakeData that looks like set ( LIBRARIES lib1 lib2 ...)
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/CMakeData.txt")
include("CMakeData.txt")
endif()

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

if(COMPILE_AT_FIRST)
    foreach (dec ${COMPILE_AT_FIRST})
        message("Compile library at first ${dec}" )
        add_subdirectory ( ${dec} )
    endforeach()
set(N_LIBRARIES ${COMPILE_AT_FIRST} ${N_LIBRARIES})
endif()

#only compile given libraries
if (LIBRARIES)
    foreach (dec ${LIBRARIES})
        message("Compile library ${dec}" )
        add_subdirectory ( ${dec} )
    endforeach()
else()
#get all libraries and compile them if permitted
    SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})
    foreach (dec ${SUBDIRS})
        if (N_LIBRARIES AND ";${N_LIBRARIES};" MATCHES "${dec}")
        else()
            message("Compile library ${dec}")
            add_subdirectory(${dec})
        endif()
    endforeach()
endif()

#Build modules given by CmakeData
#write that file like: set ( MODULES mod1 mod2 ...)
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/CMakeData.txt")
    include("CMakeData.txt")
endif()

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

#only compile given modules
if (MODULES)
    foreach (dec ${MODULES})
        message ("Compile module ${dec}" )
        add_subdirectory ( ${dec} )
    endforeach()
else()
#get all modules and compile them if permitted
    SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})
    foreach (dec ${SUBDIRS})
        if (N_MODULES AND ";${N_MODULES};" MATCHES "${dec}")
        else()
            message("Compile module ${dec}")
            add_subdirectory(${dec})
        endif()
    endforeach()
endif()

#Build modules given by CmakeData
#write that file like: set ( MODULES mod1 mod2 ...)
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/CMakeData.txt")
include("CMakeData.txt")
endif()

#only compile given modules
if (MODULES)
	foreach (dec ${MODULES})
		message ("Adding runtime_module ${dec}" )
                add_subdirectory ( ${dec} )
	endforeach()
else()
#get all modules and compile them if permitted
    SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})
    foreach (dec ${SUBDIRS})
        if (N_MODULES AND ";${N_MODULES};" MATCHES "${dec}")
        else()
            add_subdirectory(${dec})
        endif()
    endforeach()
endif()

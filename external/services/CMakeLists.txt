# Build services given by CMakeData.txt
# Create CMakeData that looks like set ( SERVICES service1 service2 ...)
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/CMakeData.txt")
    include("CMakeData.txt")
endif()

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

if(COMPILE_AT_FIRST)
    foreach (dec ${SERVICES_COMPILE_AT_FIRST})
        message ("Compile service at first ${dec}" )
        add_subdirectory ( ${dec} )
    endforeach()
set(N_SERVICES ${SERVICES_COMPILE_AT_FIRST} ${N_SERVICES})
endif()

# Only compile given services
if (SERVICES)
    foreach (dec ${SERVICES})
        message ("Compile service ${dec}" )
        add_subdirectory ( ${dec} )
    endforeach()
else()
# Get all services and compile them if permitted
    SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})
    foreach (dec ${SUBDIRS})
        if (N_SERVICES AND ";${N_SERVICES};" MATCHES "${dec}")
        else()
            message("Compile services ${dec}")
            add_subdirectory(${dec})
        endif()
    endforeach()
endif()

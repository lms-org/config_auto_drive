set ( SOURCES 
	"src/street_environment.cpp"
    "src/obstacle.cpp"
)

set (HEADERS 
	"include/street_environment/street_environment.h"
    "include/street_environment/obstacle.h"
    "include/street_environment/road.h"
)

include_directories("include")
add_library (street_environment SHARED ${SOURCES} ${HEADERS})
#target_link_libraries (street_environment PRIVATE lms)

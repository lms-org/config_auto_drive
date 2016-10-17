set ( SOURCES
	"src/street_obstacle_merger.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/street_obstacle_merger.h"
)
include_directories("include")

add_library (street_obstacle_merger MODULE ${SOURCES} ${HEADERS})
target_link_libraries(street_obstacle_merger PRIVATE lmscore math_lib lms_sensor_utils street_environment area_of_detection phoenix_CC2016_service)

set ( SOURCES
	"src/street_obstacle_ir_detector.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/street_obstacle_ir_detector.h"
)
include_directories("include")

add_library (street_obstacle_ir_detector MODULE ${SOURCES} ${HEADERS})
target_link_libraries(street_obstacle_ir_detector PRIVATE lmscore math_lib imaging street_environment phoenix_CC2016_service local_course)

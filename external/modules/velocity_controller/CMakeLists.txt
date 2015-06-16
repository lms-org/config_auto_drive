set ( SOURCES 
	"src/velocity_controller.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/velocity_controller.h"
)
include_directories("include")

add_library (velocity_controller MODULE ${SOURCES} ${HEADERS})
target_link_libraries(velocity_controller PRIVATE lms math_lib sensor_utils)

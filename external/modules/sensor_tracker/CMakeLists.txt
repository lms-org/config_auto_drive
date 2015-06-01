set ( SOURCES 
        "src/sensor_tracker.cpp"
	"src/interface.cpp"
)

set (HEADERS
        "include/sensor_tracker.h"
)
include_directories("include")

add_library (sensor_tracker MODULE ${SOURCES} ${HEADERS})
target_link_libraries(sensor_tracker PRIVATE lms sensor_utils imaging)

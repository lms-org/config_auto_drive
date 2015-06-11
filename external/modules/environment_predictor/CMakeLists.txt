set ( SOURCES 
	"src/interface.cpp"
        "src/environment_predictor.cpp"
)

set (HEADERS
        "include/environment_predictor.h"
	${HEADERS_SHARED}
)

include_directories("include")
include_directories("kalman_filter_statetrans")
include_directories("project_points")
file(GLOB kalman "kalman_filter_statetrans/*.c" "kalman_filter_statetrans/*.h")
#file(GLOB project_points "project_points/*.c" "project_points/*.h")
add_library ( environment_predictor MODULE ${SOURCES} ${HEADERS} ${kalman})
target_link_libraries(environment_predictor PRIVATE lms imaging imaging_detection sensor_utils)

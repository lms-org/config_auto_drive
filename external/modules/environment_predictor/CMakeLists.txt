set ( SOURCES
	"src/interface.cpp"
        "src/environment_predictor.cpp"
)

set (HEADERS
        "include/environment_predictor.h"
	${HEADERS_SHARED}
)

set(KALMAN "Matlab/codegen/lib/kalman_filter_lr")
include_directories("include")
include_directories("${KALMAN}")
file(GLOB kalman "${KALMAN}/*.cpp" "${KALMAN}/*.h")
add_library ( environment_predictor MODULE ${SOURCES} ${HEADERS} ${kalman})
target_link_libraries(environment_predictor PRIVATE lmscore imaging imaging_detection sensor_utils)

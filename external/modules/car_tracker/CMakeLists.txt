set ( SOURCES
	"src/car_tracker.cpp"
	"src/interface.cpp"
)

set (HEADERS
        "include/car_tracker.h"
        "include/filter/system_model.h"
        "include/filter/measurement_model.h"
)
include_directories(
    "include"

	# Eigen3 include paths
    "/usr/include/eigen3"
	"/usr/local/include/eigen3"

	# Kalman Lib paths
	"/usr/include"
	"/usr/local/include"
)

add_library (car_tracker MODULE ${SOURCES} ${HEADERS})
target_link_libraries(car_tracker PRIVATE lmscore math_lib sensor_utils)

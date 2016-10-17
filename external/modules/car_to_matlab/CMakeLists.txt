set ( SOURCES
	"src/car_to_matlab.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/car_to_matlab.h"
)

include_directories("include")

add_library (car_to_matlab MODULE ${SOURCES} ${HEADERS})
target_link_libraries(car_to_matlab PRIVATE lmscore math_lib lms_sensor_utils socket_connection)

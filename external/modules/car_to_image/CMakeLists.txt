set ( SOURCES 
	"src/car_to_image.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/car_to_image.h"
)

include_directories("include")

add_library (car_to_image MODULE ${SOURCES} ${HEADERS})
target_link_libraries(car_to_image PRIVATE lms math_lib sensor_utils)

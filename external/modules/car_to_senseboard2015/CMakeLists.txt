set ( SOURCES
	"src/car_to_senseboard2015.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/car_to_senseboard2015.h"
)

include_directories("include")

add_library (car_to_senseboard2015 MODULE ${SOURCES} ${HEADERS})
target_link_libraries(car_to_senseboard2015 PRIVATE lmscore math_lib sensor_utils)

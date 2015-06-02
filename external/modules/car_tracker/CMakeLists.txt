set ( SOURCES 
	"src/car_tracker.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/car_tracker.h"
)

include_directories("include")

add_library (car_tracker MODULE ${SOURCES} ${HEADERS})
target_link_libraries(car_tracker PRIVATE lms math_lib)

set ( SOURCES 
	"src/test.cpp"
	"src/sensor_environment.cpp"
	"src/interface.cpp"
)

set (HEADERS 
	"include/sensor_environment/sensor_environment.h"
	"include/sensor_environment/test.h"
)

include_directories("include")

add_library ( sensor_environment_test MODULE ${SOURCES} ${HEADERS})
target_link_libraries(sensor_environment_test PRIVATE lms imaging)

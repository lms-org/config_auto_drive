set ( SOURCES 
        "src/trajectory_line_creator.cpp"
	"src/interface.cpp"
)

set (HEADERS
        "include/trajectory_line_creator.h"
)
include_directories("include")

add_library (trajectory_line_creator MODULE ${SOURCES} ${HEADERS})
target_link_libraries(trajectory_line_creator PRIVATE lms math_lib sensor_utils street_environment)

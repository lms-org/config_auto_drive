set ( SOURCES 
	"src/line_trajectory_creator.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/line_trajectory_creator.h"
)
include_directories("include")

add_library (line_trajectory_creator MODULE ${SOURCES} ${HEADERS})
target_link_libraries(line_trajectory_creator PRIVATE lms)

set ( SOURCES 
	"src/line_trajectory_follower.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/line_trajectory_follower.h"
)
include_directories("include")

add_library (line_trajectory_follower MODULE ${SOURCES} ${HEADERS})
target_link_libraries(line_trajectory_follower PRIVATE lms)

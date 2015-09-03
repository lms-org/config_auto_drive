set ( SOURCES
	"src/trajectory_point_creator.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/trajectory_point_creator.h"
)
include_directories("include")

add_library (trajectory_point_creator MODULE ${SOURCES} ${HEADERS})
target_link_libraries(trajectory_point_creator PRIVATE lmscore math_lib)

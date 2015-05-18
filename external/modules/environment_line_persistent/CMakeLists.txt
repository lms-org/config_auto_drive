set ( SOURCES 
	"src/environment_line_persistent.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/environment_line_persistent.h"
)

include_directories("include")

add_library (environment_line_persistent MODULE ${SOURCES} ${HEADERS})
target_link_libraries(environment_line_persistent PRIVATE lms math_lib)

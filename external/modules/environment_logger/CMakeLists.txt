set ( SOURCES 
	"src/environment_logger.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/environment_logger.h"
)

include_directories("include")

add_library (environment_logger MODULE ${SOURCES} ${HEADERS})
target_link_libraries(environment_logger PRIVATE lms math_lib)

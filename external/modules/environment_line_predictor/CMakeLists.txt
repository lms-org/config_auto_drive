set ( SOURCES 
	"src/environment_line_predictor.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/environment_line_predictor.h"
)

include_directories("include")

add_library (environment_line_predictor MODULE ${SOURCES} ${HEADERS})
target_link_libraries(environment_line_predictor PRIVATE lms math_lib)

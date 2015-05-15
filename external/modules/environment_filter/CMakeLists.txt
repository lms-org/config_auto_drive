set ( SOURCES 
	"src/image_hint_filter.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/image_hint_filter.h"
)

include_directories("include")

add_library (environment_filter MODULE ${SOURCES} ${HEADERS})
target_link_libraries(environment_filter PRIVATE lms math_lib)

set ( SOURCES 
	"src/image_hint_merger.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/image_hint_merger.h"
)

include_directories("include")

add_library (image_hint_merger MODULE ${SOURCES} ${HEADERS})
target_link_libraries(image_hint_merger PRIVATE lms imaging imaging_detection)

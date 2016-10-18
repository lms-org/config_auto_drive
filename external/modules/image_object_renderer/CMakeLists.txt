set ( SOURCES
	"src/image_object_renderer.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/image_object_renderer.h"
)
include_directories("include")

add_library (image_object_renderer MODULE ${SOURCES} ${HEADERS})
target_link_libraries(image_object_renderer PRIVATE lmscore lms_imaging street_environment)

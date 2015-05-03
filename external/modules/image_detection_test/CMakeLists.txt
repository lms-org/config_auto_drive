set ( SOURCES 
	"src/test.cpp"
	"src/image_detection.cpp"
	"src/interface.cpp"
)

set (HEADERS 
	"include/image_detection/image_detection.h"
	"include/image_detection/test.h"
)

include_directories("include")

add_library ( image_detection_test MODULE ${SOURCES} ${HEADERS})
target_link_libraries(image_detection_test PRIVATE lms imaging)

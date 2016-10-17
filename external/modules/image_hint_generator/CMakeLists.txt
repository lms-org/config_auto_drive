set ( SOURCES
	"src/image_hint_generator.cpp"
	"src/interface.cpp"
)

set (HEADERS
	"include/image_hint_generator.h"
)

include_directories("include")

add_library (image_hint_generator MODULE ${SOURCES} ${HEADERS})
target_link_libraries(image_hint_generator PRIVATE lmscore math_lib imaging
    lms_imaging_detection phoenix_CC2016_service street_environment)

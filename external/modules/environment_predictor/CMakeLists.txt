set ( SOURCES 
	"src/interface.cpp"
        "src/environment_predictor.cpp"
)

set (HEADERS
        "include/environment_predictor.h"
	${HEADERS_SHARED}
)

include_directories("include")
include_directories("kalman_filter_lr")
file(GLOB kalman_c "kalman_filter_lr/*.c")
file(GLOB kalman_h "kalman_filter_lr/*.h")
#set_source_files_properties(${kalman_c} PROPERTIES LANGUAGE CXX)
#set_source_files_properties(${kalman_h} PROPERTIES LANGUAGE CXX)
add_library ( environment_predictor MODULE ${SOURCES} ${HEADERS} ${kalman_c} ${kalman_h})
target_link_libraries(environment_predictor PRIVATE lms imaging imaging_detection)

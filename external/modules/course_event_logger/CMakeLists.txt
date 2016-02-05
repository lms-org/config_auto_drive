set(SOURCES
    "src/course_event_logger.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/course_event_logger.h"
)

include_directories(include)
add_library(course_event_logger MODULE ${SOURCES} ${HEADERS})
target_link_libraries(course_event_logger PRIVATE lmscore sensor_utils)

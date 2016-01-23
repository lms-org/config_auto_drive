set(SOURCES
    "src/set_control.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/set_control.h"
)

include_directories(include)
add_library(set_control MODULE ${SOURCES} ${HEADERS})
target_link_libraries(set_control PRIVATE lmscore sensor_utils)

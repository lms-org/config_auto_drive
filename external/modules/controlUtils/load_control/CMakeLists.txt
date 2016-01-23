set(SOURCES
    "src/load_control.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/load_control.h"
)

include_directories(include)
add_library(load_control MODULE ${SOURCES} ${HEADERS})
target_link_libraries(load_control PRIVATE lmscore sensor_utils)

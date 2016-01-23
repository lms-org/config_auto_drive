set(SOURCES
    "src/save_control.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/save_control.h"
)

include_directories(include)
add_library(save_control MODULE ${SOURCES} ${HEADERS})
target_link_libraries(save_control PRIVATE lmscore)

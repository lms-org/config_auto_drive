set(SOURCES
    "src/area_of_detection.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/area_of_detection/area_of_detection.h"
)

include_directories(include)
add_library(area_of_detection SHARED ${SOURCES} ${HEADERS})
target_link_libraries(area_of_detection PRIVATE lmscore math_lib)

set(SOURCES
    "src/obstacle_to_local_course.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/obstacle_to_local_course.h"
)

include_directories(include)
add_library(obstacle_to_local_course MODULE ${SOURCES} ${HEADERS})
target_link_libraries(obstacle_to_local_course PRIVATE lmscore local_course street_environment)

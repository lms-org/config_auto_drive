set(SOURCES
    "src/obstacle_from_laser.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/obstacle_from_laser.h"
)

include_directories(include)
add_library(obstacle_from_laser MODULE ${SOURCES} ${HEADERS})
target_link_libraries(obstacle_from_laser PRIVATE lmscore street_environment lms_math)

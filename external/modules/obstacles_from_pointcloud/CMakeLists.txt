set(SOURCES
    "src/obstacle_from_laser.cpp"
    "src/interface.cpp"
    "src/pointcloud_filter.cpp"
)

set(HEADERS
    "include/obstacle_from_laser.h"
    "include/pointcloud_filter.h"
)

include_directories(include)
add_library(obstacle_from_pointcloud MODULE ${SOURCES} ${HEADERS})
target_link_libraries(obstacle_from_pointcloud PRIVATE lmscore street_environment lms_math)

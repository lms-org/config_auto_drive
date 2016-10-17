set(SOURCES
    "src/lap_time.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/lap_time.h"
)

include_directories(include)
add_library(lap_time MODULE ${SOURCES} ${HEADERS})
target_link_libraries(lap_time PRIVATE lmscore lms_sensor_utils street_environment)

set(SOURCES
    "src/car_track_visulizer.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/car_track_visulizer.h"
)

include_directories(include)
add_library(car_track_visulizer MODULE ${SOURCES} ${HEADERS})
target_link_libraries(car_track_visulizer PRIVATE lmscore)

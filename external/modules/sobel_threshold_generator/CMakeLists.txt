set(SOURCES
    "src/sobel_threshold_generator.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/sobel_threshold_generator.h"
)

include_directories(include)
add_library(sobel_threshold_generator MODULE ${SOURCES} ${HEADERS})
target_link_libraries(sobel_threshold_generator PRIVATE lmscore imaging)

set(SOURCES
    "src/sobel_threshold_generator.cpp"
    "src/fisher_breaks.cpp"
    "src/interface.cpp"
    "src/fisher_breaks.cpp"
)

set(HEADERS
    "include/sobel_threshold_generator.h"
    "include/fisher_breaks.h"
)

include_directories(include)
add_library(sobel_threshold_generator MODULE ${SOURCES} ${HEADERS})
target_link_libraries(sobel_threshold_generator PRIVATE lmscore imaging)

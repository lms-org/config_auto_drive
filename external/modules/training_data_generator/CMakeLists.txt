set(SOURCES
    "src/training_data_generator.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/training_data_generator.h"
)

include_directories(include)
add_library(training_data_generator MODULE ${SOURCES} ${HEADERS})
target_link_libraries(training_data_generator PRIVATE lmscore)

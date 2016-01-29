set(SOURCES
    "src/infringement_counter.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/infringement_counter.h"
)

include_directories(include)
add_library(infringement_counter MODULE ${SOURCES} ${HEADERS})
target_link_libraries(infringement_counter PRIVATE lmscore)

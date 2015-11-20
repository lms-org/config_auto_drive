set(SOURCES
    "src/socket_datatype_definer.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/socket_datatype_definer.h"
)

include_directories(include)
add_library(socket_datatype_definer MODULE ${SOURCES} ${HEADERS})
target_link_libraries(socket_datatype_definer PRIVATE lmscore sensor_utils imaging)

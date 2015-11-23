set(SOURCES
    "src/sound_controller.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/sound_controller.h"
)

include_directories(include)
add_library(sound_controller MODULE ${SOURCES} ${HEADERS})
target_link_libraries(sound_controller PRIVATE lmscore)

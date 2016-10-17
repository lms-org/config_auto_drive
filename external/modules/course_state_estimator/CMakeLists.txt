set(SOURCES
    "src/course_state_estimator.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/course_state_estimator.h"
)

include_directories(include)
add_library(course_state_estimator MODULE ${SOURCES} ${HEADERS})
target_link_libraries(course_state_estimator PRIVATE lmscore street_environment)

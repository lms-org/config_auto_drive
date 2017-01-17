set(SOURCES
    "src/new_road_detection.cpp"
    "src/crossing_detection.cpp"
    "src/obstacle_detection.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/new_road_detection.h"
    "include/crossing_detection.h"
    "include/obstacle_detection.h"
    "include/detection_utils.h"
)

include_directories(include)
add_library(new_road_detection MODULE ${SOURCES} ${HEADERS})


target_link_libraries(new_road_detection PRIVATE lmscore local_course street_environment lms_imaging lms_imaging_detection phoenix_CC2016_service)# ${CONAN_LIBS})# opencv_calib3d opencv_core opencv_highgui)

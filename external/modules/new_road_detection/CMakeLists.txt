set(SOURCES
    "src/new_road_detection.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/new_road_detection.h"
)

include_directories(include)
add_library(new_road_detection MODULE ${SOURCES} ${HEADERS})

find_package( OpenCV REQUIRED )

target_link_libraries(new_road_detection PRIVATE lmscore local_course street_environment lms_imaging lms_imaging_detection ${OpenCV_LIBS})# ${CONAN_LIBS})# opencv_calib3d opencv_core opencv_highgui)

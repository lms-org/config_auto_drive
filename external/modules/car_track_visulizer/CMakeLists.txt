set(SOURCES
    "src/car_track_visulizer.cpp"
    "src/main_window.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/car_track_visulizer.h"
    "include/main_window.h"
)


#QT
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Charts)


include_directories(include)
add_library(car_track_visulizer MODULE ${SOURCES} ${HEADERS})
target_link_libraries(car_track_visulizer PRIVATE lmscore street_environment Qt5::Widgets Qt5::Charts Qt5Core)

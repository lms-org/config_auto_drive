set(SOURCES
    "src/qt_gui_service.cpp"
    "src/interface.cpp"
    "src/qt_gui_service_updater.cpp"
)

set(HEADERS
    "include/qt_gui_service/qt_gui_service.h"
    "include/qt_gui_service/qt_gui_service_updater.h"
)
#QT
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Widgets)

include_directories(include)
add_library(qt_gui_service SHARED ${SOURCES} ${HEADERS})
target_link_libraries(qt_gui_service PRIVATE lmscore Qt5Core Qt5::Widgets)

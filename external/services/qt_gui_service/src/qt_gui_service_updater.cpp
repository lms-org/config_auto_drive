#include "qt_gui_service/qt_gui_service_updater.h"
#include "qt_gui_service/qt_gui_service.h"

namespace qt_gui_service{
bool QtGuiServiceUpdater::initialize() {
    return true;
}

bool QtGuiServiceUpdater::deinitialize() {
    return true;
}

bool QtGuiServiceUpdater::cycle() {
    lms::ServiceHandle<QtGuiService> service = getService<QtGuiService>("QT_GUI_SERVICE");
    if(!service.isValid()){
        logger.error("cycle")<<"QT_GUI_SERVICE is invalid!";
        return false;
    }
    service->process();
    return true;
}
} //namespace qt_gui_service

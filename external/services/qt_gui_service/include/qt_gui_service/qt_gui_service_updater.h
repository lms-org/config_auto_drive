#ifndef QT_GUI_SERVICE_UPDATER_H
#define QT_GUI_SERVICE_UPDATER_H

#include <lms/module.h>
namespace qt_gui_service{

/**
 * @brief LMS module car_track_visulizer
 **/
class QtGuiServiceUpdater : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};
}
#endif // QT_GUI_SERVICE_UPDATER_H

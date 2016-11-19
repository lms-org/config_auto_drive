#ifndef QT_GUI_SERVICE_H
#define QT_GUI_SERVICE_H

#include <lms/service.h>
#include <QtGui/QGuiApplication>
#include <QtWidgets/QApplication>

namespace qt_gui_service {

/**
 * @brief LMS service qt_gui_service
 **/
class QtGuiService : public lms::Service {

    QGuiApplication* guiApp;
    QApplication* app;
    int argc;
    char** argv;
public:
    bool init() override;
    void destroy() override;
    void process();
};

} // namespace qt_gui_service

#endif // QT_GUI_SERVICE_H

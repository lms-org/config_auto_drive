#include "qt_gui_service/qt_gui_service.h"
#include <string>

namespace qt_gui_service {

bool QtGuiService::init() {
    //std::vector<std::string> qArgs = config().getArray<std::string>("QArgs");
    int argc = 0;//qArgs.size();
    char** argv = nullptr;
    if(argc > 0){
        //TODO argv;
    }
    app = new QApplication(argc, argv);
    setlocale(LC_NUMERIC,"C"); //because of http://doc.qt.io/qt-4.8/qcoreapplication.html
    logger.info("init")<<"QGuiApplication started";
    return true;
}


void QtGuiService::process(){
    app->processEvents();
}



void QtGuiService::destroy() {
    //app->exit();

}

} // namespace qt_gui_service

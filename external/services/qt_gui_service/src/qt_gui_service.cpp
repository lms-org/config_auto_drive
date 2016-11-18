#include "qt_gui_service/qt_gui_service.h"
#include <string>

namespace qt_gui_service {

bool QtGuiService::init() {
    //std::vector<std::string> qArgs = config().getArray<std::string>("QArgs");
    /*

    Warning: The data referred to by argc and argv must stay valid for the entire lifetime of the QApplication object.
 In addition, argc must be greater than zero and argv must contain at least one valid character string.

     *
     */
    argc = 0;//qArgs.size();
    argv = nullptr;
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

#include "course_event_logger.h"

bool CourseEventLogger::initialize() {
    //setting up saving
    if(!isEnableSave()){
        return false;
    }
    std::string dir = saveLogDir("course_event_logger");
    courseCsv.open(dir +"course.csv");
    eventCsv.open(dir +"events.csv");

    //data
    car = readChannel<street_environment::Car>("CAR");
    return true;
}

bool CourseEventLogger::deinitialize() {
    courseCsv.close();
    eventCsv.close();
    return true;
}

bool CourseEventLogger::cycle() {
    for(std::string s : messaging()->receive("COURSE_EVENT")){
        //print s to x,y
        eventCsv<<car->position().x<<","<< car->position().y<<","<<s;
    }
    courseCsv<<car->position().x<<","<< car->position().y<<","<<car->velocity()<< "," <<car->targetSpeed() << std::endl;

    return true;
}

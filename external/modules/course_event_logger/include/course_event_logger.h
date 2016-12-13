#ifndef COURSE_EVENT_LOGGER_H
#define COURSE_EVENT_LOGGER_H

#include <lms/module.h>
#include "street_environment/car.h"
#include <ostream>
/**
 * @brief LMS module course_event_logger
 **/
class CourseEventLogger : public lms::Module {
    lms::ReadDataChannel<street_environment::CarCommand> car;

    std::ofstream courseCsv;
    std::ofstream eventCsv;
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // COURSE_EVENT_LOGGER_H

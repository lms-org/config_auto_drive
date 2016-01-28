#ifndef OBSTACLE_TO_LOCAL_COURSE_H
#define OBSTACLE_TO_LOCAL_COURSE_H

#include <lms/module.h>
#include "local_course/local_course.h"
#include "street_environment/crossing.h"

/**
 * @brief LMS module obstacle_to_local_course
 **/
class ObstacleToLocalCourse : public lms::Module {
    lms::ReadDataChannel<street_environment::EnvironmentObjects> environment;
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // OBSTACLE_TO_LOCAL_COURSE_H

#ifndef COURSE_STATE_ESTIMATOR_H
#define COURSE_STATE_ESTIMATOR_H

#include <lms/module.h>
#include "street_environment/road.h"

/**
 * @brief LMS module course_state_estimator
 **/
class CourseStateEstimator : public lms::Module {
    lms::ReadDataChannel<street_environment::EnvironmentObjects> environment;
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // COURSE_STATE_ESTIMATOR_H

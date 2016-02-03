#ifndef COURSE_STATE_ESTIMATOR_H
#define COURSE_STATE_ESTIMATOR_H

#include <lms/module.h>

/**
 * @brief LMS module course_state_estimator
 **/
class CourseStateEstimator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // COURSE_STATE_ESTIMATOR_H

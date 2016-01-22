#ifndef LOCAL_COURSE_H
#define LOCAL_COURSE_H

#include <lms/service.h>
#include "lms/math/vertex.h"
#include <vector>
#include "matlab_kalman.h"
#include "street_environment/road.h"

namespace local_course {

/**
 * @brief LMS service local_course
 **/
class LocalCourse : public lms::Service {
    MatlabKalman kalman;
    std::vector<lms::math::vertex2f> pointsToAdd;
public:
    LocalCourse();
    bool init() override;
    void destroy() override;

    void update(float d, float dy, float dphi);
    void addPoints(const std::vector<lms::math::vertex2f> &points);
    void addPoint(const lms::math::vertex2f &p);
    street_environment::RoadLane getCourse();
    street_environment::RoadLane getCourse(lms::Time time);
    void resetData();
    void configsChanged() override;
    std::vector<lms::math::vertex2f> getPointsToAdd();
};

} // namespace local_course

#endif // LOCAL_COURSE_H

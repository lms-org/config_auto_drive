#ifndef LOCAL_COURSE_H
#define LOCAL_COURSE_H

#include <lms/service.h>
#include "lms/math/vertex.h"
#include <vector>
#include "matlab_kalman.h"
#include "street_environment/road.h"
#include "lms/math/lookup_table.h"


struct LineX;

namespace local_course {


/**
 * @brief LMS service local_course
 **/
class LocalCourse : public lms::Service {
    LineX *lineX;

    //TODO one object for pointsToAdd and weights
    std::vector<lms::math::vertex2f> pointsToAdd;
    std::vector<float> weights;
    std::vector<lms::math::vertex2f> pointsAdded;
    int outlierStartingState;
    float outlierPercentile;
    float outlierPercentileMultiplier;
    int resetCounter;

public:
    LocalCourse();
    bool init() override;
    void destroy() override;

    void update(float dx, float dy, float dphi);
    bool addPoints(const std::vector<lms::math::vertex2f> &points);
    bool addPoints(const std::vector<lms::math::vertex2f> &points,const std::vector<float> &weights);
    bool addPoint(const lms::math::vertex2f &p);
    bool addPoint(const lms::math::vertex2f &p,const float &weight);
    street_environment::RoadLane getCourse();
    street_environment::RoadLane getCourse(lms::Time time);
    void resetData();
    void configsChanged() override;
    std::vector<lms::math::vertex2f> getPointsToAdd();
    std::vector<lms::math::vertex2f> getPointsAdded();
};

} // namespace local_course

#endif // LOCAL_COURSE_H

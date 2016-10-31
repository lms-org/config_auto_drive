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

    MatlabKalman kalman;
    std::vector<lms::math::vertex2f> pointsToAdd;
    std::vector<lms::math::vertex2f> pointsAdded;
    int outlierStartingState;
    float outlierPercentile;
    float outlierPercentileMultiplier;
    int resetCounter;

    lms::math::LookupTable<float,lms::math::LookupTableOrder::ASC> m_thresholdLookup;
    void filterPoints();

public:
    LocalCourse();
    bool init() override;
    void destroy() override;

    void update(float dx, float dy, float dphi);
    void addPoints(const std::vector<lms::math::vertex2f> &points);
    void addPoint(const lms::math::vertex2f &p);
    street_environment::RoadLane getCourse();
    street_environment::RoadLane getCourse(lms::Time time);
    void resetData();
    void configsChanged() override;
    std::vector<lms::math::vertex2f> getPointsToAdd();
    std::vector<lms::math::vertex2f> getPointsAdded();
    void distanceLinePoint(lms::math::vertex2f P, lms::math::vertex2f Q, lms::math::vertex2f M, float *dst, float *lambda);
    float thresholdFunction(float s);
};

} // namespace local_course

#endif // LOCAL_COURSE_H

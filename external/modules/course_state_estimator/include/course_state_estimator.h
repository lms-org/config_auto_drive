#ifndef COURSE_STATE_ESTIMATOR_H
#define COURSE_STATE_ESTIMATOR_H

#include <lms/module.h>
#include <lms/math/vertex.h>
#include <lms/math/mathEigen.h>
#include <street_environment/road.h>

/**
 * @brief LMS module course_state_estimator
 **/
class CourseStateEstimator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;


private:
    Eigen::Vector3f states;
    Eigen::Matrix3f transition;
    //! [straight, straight-curve, curve]
    Eigen::Vector3f observation;
    float observationProbability1;
    float observationProbability2;
    float curvatureThreshold;
    float distance1;
    float distance2;
    float distance3;
    float distance4;

    lms::ReadDataChannel<street_environment::RoadLane> road;

    void mapObservations();
    void update();
    void mapObservationProbability(const float& curvature, Eigen::Vector2f& probability, float observationProbability);
    float calculateCurvature(float minDistance, float maxDistance);
    lms::math::vertex2f interpolateRoadAtDistance(float distanceIn);

};

#endif // COURSE_STATE_ESTIMATOR_H

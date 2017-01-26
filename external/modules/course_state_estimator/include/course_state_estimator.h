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
    //first state is straight, second straight-curve, third curve
    lms::ReadDataChannel<street_environment::RoadLane> road;
    lms::WriteDataChannel<street_environment::RoadStates> roadStates;
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;


private:
    Eigen::Vector3f probabilityStates;
    Eigen::Matrix3f transition;
    //! [straight, straight-curve, curve]
    Eigen::Vector3f observation;
    float observationProbability1;
    float observationProbability2;
    float curvatureThreshold;
    //distance1/2 is the first part of the road, distance 3/4 the second
    float distance1;
    float distance2;
    float distance3;
    float distance4;

    float curvaturePT1 = 0;

    void mapObservations();
    void update();//TODO toller Name
    void mapObservationProbability(const float& curvature, Eigen::Vector2f& probability, float observationProbability);
    Eigen::Vector3f emissionProbabilitiesStraight();
    float calculateCurvature(float minDistance, float maxDistance);
    street_environment::RoadState getStateFromIndex(int index);

};

#endif // COURSE_STATE_ESTIMATOR_H

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
    lms::ReadDataChannel<street_environment::EnvironmentObjects> environment;
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
    //TODO entweder sinnvolle namen oder eine beschreibung dazu.
    float distance1;
    float distance2;
    float distance3;
    float distance4;

    float curvaturePT1 = 0;

    lms::ReadDataChannel<street_environment::RoadLane> road;

    void mapObservations();
    void update();//TODO toller Name
    void mapObservationProbability(const float& curvature, Eigen::Vector2f& probability, float observationProbability);
    Eigen::Vector3f emissionProbabilitiesStraight();
    float calculateCurvature(float minDistance, float maxDistance);
    street_environment::RoadState getStateFromIndex(int index);

};

#endif // COURSE_STATE_ESTIMATOR_H
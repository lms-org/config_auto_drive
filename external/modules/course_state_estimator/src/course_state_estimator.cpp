#include "course_state_estimator.h"

bool CourseStateEstimator::initialize() {
    environment = readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT");
    return true;
}

bool CourseStateEstimator::deinitialize() {
    return true;
}

bool CourseStateEstimator::cycle() {
    for(street_environment::EnvironmentObjectPtr envPtr: environment->objects){
        lms::math::polyLine2f allPoints;
        if(envPtr->getType() == street_environment::RoadLane::TYPE){
            street_environment::RoadLanePtr roadPtr = std::static_pointer_cast<street_environment::RoadLane>(envPtr);
            if(roadPtr->points().size() == 0)
                continue;
            lms::math::vertex2f centerOfMass = lms::math::vertex2f(0,0);
            for(const lms::math::vertex2f &v: roadPtr->points()){
                centerOfMass =centerOfMass+ v;
            }
            centerOfMass = centerOfMass/roadPtr->points().size();

            for(const lms::math::vertex2f &v: roadPtr->points()){
                allPoints.points().push_back(v-centerOfMass);
            }
        }
    }
    return true;
}

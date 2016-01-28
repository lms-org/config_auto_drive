#include "obstacle_to_local_course.h"

bool ObstacleToLocalCourse::initialize() {
    environment = readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT");
    return true;
}

bool ObstacleToLocalCourse::deinitialize() {
    return true;
}

bool ObstacleToLocalCourse::cycle() {
    float streetWidth= config().get<float>("streetWidth",0.8);
    lms::ServiceHandle<local_course::LocalCourse> localCourse = getService<local_course::LocalCourse>("LOCAL_COURSE_SERVICE");
    for(street_environment::EnvironmentObjectPtr envPtr:environment->objects){
        if(envPtr->getType() == street_environment::Crossing::TYPE){
            if(envPtr->trust() > config().get<float>("obstacleMinTrust",0.8)){
                street_environment::CrossingPtr crossPtr=std::static_pointer_cast<street_environment::Crossing>(envPtr);
                lms::math::vertex2f midPos = crossPtr->position() + crossPtr->viewDirection().rotateAntiClockwise90deg()*streetWidth/4;
                int virtualCrossingPoints =config().get<int>("virtualCrossingPoints",1);
                float step = streetWidth/virtualCrossingPoints;
                logger.debug("cycle")<<crossPtr->position()<<" viewDir"<< crossPtr->viewDirection()<<" midPos"<<midPos;

                for(int i = 0; i< virtualCrossingPoints;i++){
                    lms::math::vertex2f newPoint= midPos +crossPtr->viewDirection()*step*i;
                    localCourse->addPoint(newPoint);
                    logger.debug("adding point")<<newPoint;
                }
            }
        }
    }
    return true;
}

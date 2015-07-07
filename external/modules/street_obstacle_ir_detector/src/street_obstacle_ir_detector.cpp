#include "street_obstacle_ir_detector.h"
#include "lms/datamanager.h"

bool StreetObstacleIRDetector::initialize() {
    config = getConfig();
    distanceSensor = datamanager()->readChannel<sensor_utils::DistanceSensor>(this,"DISTANCE_SENSOR");
    env = datamanager()->writeChannel<street_environment::EnvironmentObstacles>(this,"ENVIRONMENT");
    return true;
}

bool StreetObstacleIRDetector::deinitialize() {
    return true;
}

bool StreetObstacleIRDetector::cycle() {
    float totalDistance = distanceSensor->totalX();
    float obstacleTriggerDistance = config->get<float>("obstacleTriggerDistance",0.1);
    float obstacleWidth = config->get<float>("obstacleWidth",0.35);

    if(totalDistance < obstacleTriggerDistance){
        //found some obstacles in reach
        std::shared_ptr<street_environment::Obstacle> obstacle(new street_environment::Obstacle());
        obstacle->updatePosition(lms::math::vertex2f(0,totalDistance+obstacleWidth/2));
        env->objects.push_back(obstacle);
    }else{
        //no obstacle in reach
    }


    return true;
}

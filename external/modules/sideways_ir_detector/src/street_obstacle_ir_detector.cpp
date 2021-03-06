#include "street_obstacle_ir_detector.h"
#include "phoenix_CC2016_service/phoenix_CC2016_service.h"
#include "local_course/local_course.h"

bool StreetObstacleIRDetector::initialize() {

    sensors = readChannel<sensor_utils::SensorContainer>("SENSORS");
    env = writeChannel<street_environment::EnvironmentObjects>("ENVIRONMENT");
    return true;
}

bool StreetObstacleIRDetector::deinitialize() {
    return true;
}

bool StreetObstacleIRDetector::cycle() {
    lms::ServiceHandle<phoenix_CC2016_service::Phoenix_CC2016Service> phoenixService = getService<phoenix_CC2016_service::Phoenix_CC2016Service>("PHOENIX_SERVICE");
    if(phoenixService->driveMode() != phoenix_CC2016_service::CCDriveMode::FMH){
        return true;
    }
    //check if we are on the right side #HACK
    lms::ServiceHandle<local_course::LocalCourse> localCourse = getService<local_course::LocalCourse>("LOCAL_COURSE_SERVICE");
    if(localCourse.isValid()){
        if(localCourse->getCourse().points()[3].y > 0){
            return true;
        }
    }else{
        logger.warn("No valid LocalCourse Service");
    }
    if(sensors->hasSensor("LIDAR")){
        std::shared_ptr<sensor_utils::DistanceSensor> lidar = sensors->sensor<sensor_utils::DistanceSensor>("LIDAR");
        float distance = lidar->distance;
        float obstacleMinDistance = config().get<float>("obstacleMinDistance",0.01);
        float obstacleTriggerDistance = config().get<float>("obstacleTriggerDistance",0.20);
        float obstacleWidth = config().get<float>("obstacleWidth",0.1);
        logger.debug("distance")<<distance;

        if(distance > obstacleMinDistance &&distance < obstacleTriggerDistance){
            logger.error("distance")<<distance;
            //found some obstacles in reach
            std::shared_ptr<street_environment::Obstacle> obstacle(new street_environment::Obstacle());
            lms::math::vertex2f pos = lms::math::vertex2f(lidar->totalX(),lidar->totalY() - obstacleWidth/2);
            obstacle->addPoint(pos);
            obstacle->setTrust(config().get<float>("obstacleInitTrust",0.5));
            obstacle->width(obstacleWidth);
            //TODO obstacle->viewDirection(lms::math::vertex2f(1,0));
            env->objects.push_back(obstacle);
        }else{
            //no obstacle in reach
        }
    }else{
        logger.warn("cycle")<<"no valid sensor given";
    }

    return true;
}

#include "obstacle_from_laser.h"
#include <street_environment/obstacle.h>

bool ObstacleFromPointCloud::initialize() {
    pointCloud = readChannel<lms::math::PointCloud2f>("POINT_CLOUD");
    env = writeChannel<street_environment::EnvironmentObjects>("ENVIRONMENT");
    return true;
}

bool ObstacleFromPointCloud::deinitialize() {
    return true;
}

bool ObstacleFromPointCloud::cycle() {
    //clear old objects
    if(!pointCloud.hasNewData()){
        logger.debug("no new data available");
        return true;
    }
    env->objects.clear();
    if(pointCloud->points().size() == 0){
        logger.debug("cycle")<<"no points given!";
        return true;
    }

    float blobMaxDistance = config().get<float>("blobMaxDistance",0.1);
    std::vector<std::vector<lms::math::vertex2f>> blobs;
    std::vector<lms::math::vertex2f> blob;
    lms::math::vertex2f lastPoint = pointCloud->points()[0];
    blob.push_back(lastPoint);
    for(int i = 1; i < (int)pointCloud->points().size();i++){
        if(lastPoint.distance(pointCloud->points()[i])<=blobMaxDistance){
            lastPoint = pointCloud->points()[i];
            blob.push_back(lastPoint);
        }else{
            blobs.push_back(blob);
            blob.clear();
            lastPoint = pointCloud->points()[i];
            blob.push_back(lastPoint);
        }
    }
    blobs.push_back(blob); //add the last blob
    logger.debug("cycle")<<"found "<<blobs.size()<<" blobs";
    int minBlobElements = config().get<int>("minBlobElements",5);
    float obstacleWidth = 0.3;
    //check found blobs
    for(const std::vector<lms::math::vertex2f> &vb:blobs){
        if((int)vb.size() >= minBlobElements){
            //found some obstacles in reach
            std::shared_ptr<street_environment::Obstacle> obstacle(new street_environment::Obstacle());
            for(const lms::math::vertex2f &v:vb){
                obstacle->addPoint(v);
            }
            //TODO besserer create trust value
            obstacle->setTrust(config().get<float>("obstacleInitTrust",0.8));
            obstacle->width(obstacleWidth);
            obstacle->addSensor("LIDAR");
            env->objects.push_back(obstacle);
        }
    }
    logger.debug("cycle")<<"found "<<env->objects.size() <<" obstacles";
    env.publish();
    return true;
}

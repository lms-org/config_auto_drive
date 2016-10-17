#include "obstacle_from_laser.h"
#include <street_environment/obstacle.h>

bool ObstacleFromLaser::initialize() {
    points = readChannel<lms::math::polyLine2f> ("POINTS");
    sortedPoints = writeChannel<lms::math::polyLine2f>("SORTED_POINTS");
    road = readChannel<street_environment::RoadLane>("MIDDLE_LANE");
    env = writeChannel<street_environment::EnvironmentObjects>("ENVIRONMENT");
    return true;
}

bool ObstacleFromLaser::deinitialize() {
    return true;
}

bool ObstacleFromLaser::cycle() {
    sortedPoints->points().clear();
    if(road->points().size() == 0){
        logger.warn("Road with no points!");
    }
    //check if the point is inside the car
    for(const lms::math::vertex2f &v:points->points()){
        if(v.x < 0.1 && v.x>-0.1 && v.y < 0.05 && v.y > -0.1){
            continue;
        }
        float minDistance = 100;
        for(lms::math::vertex2f rp:road->points()){
            float ndistance = rp.distance(v);
            if(ndistance < minDistance){
                minDistance = ndistance;
            }
        }
        if(minDistance>0.4){
            continue;
        }
        sortedPoints->points().push_back(v);
    }

    //check if there are valid points
    if(sortedPoints->points().size() == 0)
        return true;

    float blobMaxDistance = config().get<float>("blobMaxDistance",0.1);
    std::vector<std::vector<lms::math::vertex2f>> blobs;
    std::vector<lms::math::vertex2f> blob;
    lms::math::vertex2f lastPoint = sortedPoints->points()[0];
    blob.push_back(lastPoint);
    for(int i = 1; i < (int)sortedPoints->points().size();i++){
        if(lastPoint.distance(sortedPoints->points()[i])<=blobMaxDistance){
            lastPoint = sortedPoints->points()[i];
            blob.push_back(lastPoint);
        }else{
            blobs.push_back(blob);
            blob.clear();
            lastPoint = sortedPoints->points()[i];
            blob.push_back(lastPoint);
        }
    }
    blobs.push_back(blob); //add the last blob

    int minBlobElements = config().get<int>("minBlobElements",5);
    float obstacleWidth = 0.3;
    //check found blobs
    for(const std::vector<lms::math::vertex2f> &vb:blobs){
        if((int)vb.size() >= minBlobElements){
            //found some obstacles in reach
            std::shared_ptr<street_environment::Obstacle> obstacle(new street_environment::Obstacle());
            lms::math::vertex2f pos = vb[vb.size()/2];
            obstacle->updatePosition(pos);
            obstacle->setTrust(config().get<float>("obstacleInitTrust",0.8));
            obstacle->width(obstacleWidth);
            obstacle->viewDirection(lms::math::vertex2f(1,0));
            env->objects.push_back(obstacle);
        }
    }

    //TODO
    return true;
}

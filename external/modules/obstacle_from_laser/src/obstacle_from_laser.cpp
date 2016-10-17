#include "obstacle_from_laser.h"

bool ObstacleFromLaser::initialize() {
    points = readChannel<lms::math::polyLine2f> ("POINTS");
    sortedPoints = writeChannel<lms::math::polyLine2f>("SORTED_POINTS");
    road = readChannel<street_environment::RoadLane>("MIDDLE_LANE");
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

    //TODO
    return true;
}

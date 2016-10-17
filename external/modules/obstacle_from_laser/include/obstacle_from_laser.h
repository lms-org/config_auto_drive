#ifndef OBSTACLE_FROM_LASER_H
#define OBSTACLE_FROM_LASER_H

#include <lms/module.h>
#include <lms/math/polyline.h>
#include <street_environment/road.h>

/**
 * @brief LMS module obstacle_from_laser
 **/
class ObstacleFromLaser : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    lms::ReadDataChannel<lms::math::polyLine2f> points;
    lms::ReadDataChannel<street_environment::RoadLane> road;

    lms::WriteDataChannel<lms::math::polyLine2f> sortedPoints;
    lms::WriteDataChannel<street_environment::EnvironmentObjects> env;
};

#endif // OBSTACLE_FROM_LASER_H

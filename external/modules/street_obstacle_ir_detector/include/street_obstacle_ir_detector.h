#ifndef STREET_OBSTACLE_IR_DETECTOR_H
#define STREET_OBSTACLE_IR_DETECTOR_H

#include "lms/module.h"
#include "street_environment/street_environment.h"
#include "street_environment/obstacle.h"
#include "lms/type/module_config.h"
#include "sensor_utils/distance_sensor.h"
class StreetObstacleIRDetector : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    street_environment::Environment *env;
    /**
     * @brief currentObstacle null if there is no obstacle currently found by the IR
     */
    //street_environment::Obstacle *currentObstacle;
    const lms::type::ModuleConfig *config;
    const sensor_utils::DistanceSensor *distanceSensor;

};

#endif /* STREET_OBSTACLE_IR_DETECTOR_H */

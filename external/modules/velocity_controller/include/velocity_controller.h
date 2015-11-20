#ifndef LMS_TRAJECTORY_POINT_CREATOR_H
#define LMS_TRAJECTORY_POINT_CREATOR_H

#include "lms/module.h"
#include "lms/math/polyline.h"
#include "lms/math/math.h"
#include "street_environment/road.h"
#include "sensor_utils/car.h"

#include "senseboard2015.h"

class VelocityController : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    lms::ReadDataChannel<street_environment::RoadLane> road;
    lms::WriteDataChannel<sensor_utils::Car> car;

    /**
     * @brief driving true the car is driving
     */
    bool driving;
    lms::extra::PrecisionTime lastCall;
    /**
     * @brief launchControll
     * @param newVeolocity
     * @param currentVelocity
     * @return true if launchControll is active, false if not
     */
    bool launchControll(float newVeolocity,float currentVelocity,sensor_utils::Car::State &state);
    bool defaultDrive(sensor_utils::Car::State &state);

};

#endif /* LMS_TRAJECTORY_POINT_CREATOR_H */

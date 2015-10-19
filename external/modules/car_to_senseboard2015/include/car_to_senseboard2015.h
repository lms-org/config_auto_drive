#ifndef CAR_TRACKER_H
#define CAR_TRACKER_H

#include "lms/module.h"
#include "comm/senseboard.h"
#include "lms/extra/time.h"
#include "sensor_utils/car.h"

class CarToSenseboard2015 : public lms::Module {

public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;

private:
    bool lastRcState;
    Comm::SensorBoard::ControlData *controlData;
    Comm::SensorBoard::SensorData *sensorData;
    const sensor_utils::Car *car;
};

#endif /* CAR_TRACKER */

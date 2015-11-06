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
    lms::WriteDataChannel<Comm::SensorBoard::ControlData> controlData;
    lms::WriteDataChannel<Comm::SensorBoard::SensorData> sensorData;
    lms::ReadDataChannel<sensor_utils::Car> car;
};

#endif /* CAR_TRACKER */

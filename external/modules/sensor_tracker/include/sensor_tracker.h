#ifndef SENSOR_TRACKER_H
#define SENSOR_TRACKER_H

#include "lms/module.h"
#include "lms/math/polyline.h"
#include "lms/math/polyline.h"
#include "street_environment/road.h"
#include "comm/senseboard.h"
#include "sensor_utils/line_depth_environment.h"

class SensorTracker : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    const Comm::SensorBoard::SensorData *sensorData;
    const Comm::SensorBoard::ControlData *controlData;
    LinedDepthEnvironment *lde;
    lms::math::polyLine2f* currentSensor;
    lms::extra::PrecisionTime last;
    bool fristRun;

};

#endif /* SENSOR_TRACKER_H */

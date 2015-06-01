#include "sensor_tracker.h"
#include "lms/datamanager.h"
#include "lms/math/math.h"
#include "lms/extra/time.h"

bool SensorTracker::initialize() {
    sensorData = datamanager()->readChannel<Comm::SensorBoard::SensorData>(this,"SENSOR_DATA");
    controlData = datamanager()->readChannel<Comm::SensorBoard::ControlData>(this,"CONTROL_DATA");
    lde = datamanager()->writeChannel<LinedDepthEnvironment>(this,"SENSOR_ENV");
    currentSensor = datamanager()->writeChannel<lms::math::polyLine2f>(this,"SENSOR_LINE");
    return true;
}

bool SensorTracker::deinitialize() {
    return true;
}
bool firstrun = true;
bool SensorTracker::cycle() {

    //get the speed of the car
    if(firstrun){
        last = lms::extra::PrecisionTime::now();
        firstrun = false;
        return true;
    }
    lms::extra::PrecisionTime delta = lms::extra::PrecisionTime::since(last);
    last = lms::extra::PrecisionTime::now();
    float velocity = controlData->control.velocity.velocity;
    float distance = velocity*delta.toFloat<std::milli>()/1000;
    lde->add(distance,sensorData->ir[sensorData->IR_SIDE_REAR]);
    return true;
}


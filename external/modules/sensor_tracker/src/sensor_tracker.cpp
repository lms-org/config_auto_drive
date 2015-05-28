#include "sensor_tracker.h"
#include "lms/datamanager.h"
#include "lms/math/math.h"
#include "lms/extra/time.h"

bool SensorTracker::initialize() {
    sensorData = datamanager()->readChannel<Comm::SensorBoard::SensorData>(this,"SENSOR_DATA");
    controlData = datamanager()->readChannel<Comm::SensorBoard::ControlData>(this,"CONTROL_DATA");
    //lde = datamanager()->writeChannel<LinedDepthEnvironment>(this,"SENSOR_ENV");
    return true;
}

bool SensorTracker::deinitialize() {
    return true;
}

bool SensorTracker::cycle() {
    /*
    //get the speed of the car
    if(last == nullptr){
        last = &lms::extra::PrecisionTime.now();
    }
    lms::extra::PrecisionTime delta = current - last;
    float velocity = controlData->control.velocity.velocity;
    */
    return true;
}


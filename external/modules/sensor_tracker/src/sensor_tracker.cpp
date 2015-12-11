#include "sensor_tracker.h"
#include "lms/math/math.h"
#include "lms/extra/time.h"

bool SensorTracker::initialize() {
    sensorData = datamanager()->readChannel<Comm::SensorBoard::SensorData>(this,"SENSOR_DATA");
    controlData = datamanager()->readChannel<Comm::SensorBoard::ControlData>(this,"CONTROL_DATA");
    lde = datamanager()->writeChannel<LinedDepthEnvironment>(this,"SENSOR_ENV");
    currentSensor = datamanager()->writeChannel<lms::math::polyLine2f>(this,"SENSOR_LINE");
    lde->set(0.05,0.02,0,INFINITY);
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
    float sensorVal = sensorData->ir[sensorData->IR_SIDE_REAR];

    logger.debug("cycle") << "speed: "<<velocity << " ir: " << sensorVal << " "<< sensorData->IR_SIDE_REAR;
    float distance = velocity*delta.toFloat<std::milli>()/1000;
    lde->add(distance,sensorVal);
    //just for debugging
    currentSensor->points().clear();
    currentSensor->points().push_back(lms::math::vertex2f(0.05,-0.05));
    float y = -0.05-sensorVal;
    logger.debug("y") << y;
    currentSensor->points().push_back(lms::math::vertex2f(0.05,y));
    return true;
}


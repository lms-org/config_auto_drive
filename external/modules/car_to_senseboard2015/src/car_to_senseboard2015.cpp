#include "car_to_senseboard2015.h"
#include <cmath>
#include <algorithm>
#include "lms/datamanager.h"
#include "lms/messaging.h"
bool CarToSenseboard2015::initialize() {
    controlData = datamanager()->writeChannel<Comm::SensorBoard::ControlData>(this,"CONTROL_DATA");
    sensorData = datamanager()->writeChannel<Comm::SensorBoard::SensorData>(this,"SENSOR_DATA");

    /*
     * HACK: it has write-Access because it should be executed before the Senseboard!
     */
    car = datamanager()->writeChannel<sensor_utils::Car>(this,"CAR");
    lastRcState = false;
    return true;
}


bool CarToSenseboard2015::deinitialize() {
    return true;
}

bool CarToSenseboard2015::cycle() {
    controlData->vel_mode = Comm::SensorBoard::ControlData::MODE_VELOCITY;
    controlData->control.velocity.velocity = car->targetSpeed;

    controlData->steering_front = car->steering_front;
    controlData->steering_rear = -car->steering_rear;

    if(sensorData->rc_on != lastRcState){
        lastRcState = sensorData->rc_on;
        //TODO broadcast msg
        messaging()->send("RC_STATE_CHANGED",std::to_string(lastRcState));
        logger.error("cycle")<<"RC_STATE_CHANGED: "<<std::to_string(lastRcState);
    }
    return true;
}

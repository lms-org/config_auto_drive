#include "car_to_senseboard2015.h"
#include <cmath>
#include <algorithm>
#include "lms/datamanager.h"

bool CarToSenseboard2015::initialize() {
    controlData = datamanager()->writeChannel<Comm::SensorBoard::ControlData>(this,"CONTROL_DATA");
    /*
     * HACK: it has write-Access because it should be executed before the Senseboard!
     */
    car = datamanager()->writeChannel<sensor_utils::Car>(this,"CAR");
    return true;
}


bool CarToSenseboard2015::deinitialize() {
    return true;
}

bool CarToSenseboard2015::cycle() {
    controlData->vel_mode = Comm::SensorBoard::ControlData::MODE_VELOCITY;
    controlData->control.velocity.velocity = car->targetSpeed;
    //TODO HACK as we don't get the speed from the car atm!
    car->velocity = car->targetSpeed;
    controlData->steering_front = car->steering_front;
    controlData->steering_rear = car->steering_rear;
    return true;
}

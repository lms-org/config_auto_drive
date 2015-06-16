#include "car_tracker.h"
#include <cmath>
#include <algorithm>
#include "lms/datamanager.h"
#include "sensor_utils/car.h"

bool CarTracker::initialize() {
    car = datamanager()->writeChannel<sensor_utils::Car>(this,"CAR");
    firstRun = true;
    delta = 0;
    return true;
}


bool CarTracker::deinitialize() {
    return true;
}

bool CarTracker::cycle() {
    if(firstRun){
        last = lms::extra::PrecisionTime::now();
        firstRun = false;
        return true;
    }
    delta = lms::extra::PrecisionTime::since(last).toFloat<std::milli>()/1000;
    logger.debug("cycle: ")<<"Delta t: "<< delta;
    DeltaState deltaTra;
    DeltaState deltaVeh;
    DeltaState deltaMouse;

    getFromVehicle(deltaTra);
    getFromVehicle(deltaVeh);
    getFromMouseSensors(deltaMouse);
    //TODO
    car->updateVelocity(car->targetSpeed,lms::math::vertex2f(1,0));
    car->updatePosition(lms::math::vertex2f(car->velocity*delta,0),lms::math::vertex2f(1,0));

    logger.debug("cycle: ")<<"speed: "<<car->targetSpeed << " deltaPos: " << car->velocity*delta;
    last = lms::extra::PrecisionTime::now();
    //TODO Kalman everything
    //TODO use EIGEN
    return true;
}

void CarTracker::getFromVehicle(DeltaState &d){
    float velocity = controlData->control.velocity.velocity;
    float steeringFront = controlData->steering_front;
    float steeringRear = controlData->steering_rear;
    //TODO
}

void CarTracker::getFromTrajectory(DeltaState &d){
    //TODO
}

void CarTracker::getFromMouseSensors(DeltaState &d){
    //TODO
}
void CarTracker::getFromImu(DeltaState &d){
    //TODO
}

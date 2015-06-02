#include "car_tracker.h"
#include <cmath>
#include <algorithm>
#include "lms/datamanager.h"

bool CarTracker::initialize() {
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
        return true;
    }
    delta = lms::extra::PrecisionTime::since(last).toFloat;
    DeltaState deltaTra;
    DeltaState deltaVeh;
    DeltaState deltaMouse;

    getFromVehicle(deltaTra);
    getFromVehicle(deltaVeh);
    getFromMouseSensors(deltaMouse);

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

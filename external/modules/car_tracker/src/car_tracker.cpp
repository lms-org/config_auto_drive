#include <cmath>
#include <algorithm>
#include "car_tracker.h"
#include "lms/datamanager.h"
#include "sensor_utils/car.h"

bool CarTracker::initialize() {
    car = datamanager()->writeChannel<sensor_utils::Car>(this,"CAR");
    controlData = datamanager()->writeChannel<Comm::SensorBoard::ControlData>(this,"CONTROL_DATA");
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
    //DeltaState deltaTra;
    DeltaState deltaVeh;
    //DeltaState deltaMouse;

    //getFromVehicle(deltaTra);
    getFromVehicle(deltaVeh);
    //getFromMouseSensors(deltaMouse);
    //TODO
    car->updateVelocity(car->targetSpeed,lms::math::vertex2f(1,0));
    lms::math::vertex2f pos = car->position;
    pos.x += deltaVeh.x;
    pos.y += deltaVeh.y;
    float angle = car->viewDirection.angle()+deltaVeh.phi;
    car->updatePosition(pos,lms::math::vertex2f(cos(angle),sin(angle)));

    logger.debug("cycle: ")<<"speed: "<<car->targetSpeed << " deltaPos: " << car->velocity*delta;
    last = lms::extra::PrecisionTime::now();
    //TODO Kalman everything
    //TODO use EIGEN
    return true;
}

void CarTracker::getFromVehicle(DeltaState &d){
    //get needed values
    float velocity = controlData->control.velocity.velocity;
    float steeringFront = controlData->steering_front;
    float steeringRear = controlData->steering_rear;
    float radstand = getConfig()->get<float>("radstand",0.26);

    //TODO vorzeichen
    d.x = velocity*delta*cos(steeringRear);
    d.y = velocity*delta*sin(steeringRear);
    d.phi = velocity*delta/radstand*sin(steeringFront-steeringRear)/cos(steeringRear);

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

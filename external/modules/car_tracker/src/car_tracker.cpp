#include <cmath>
#include <algorithm>
#include "car_tracker.h"
#include "lms/datamanager.h"
#include "sensor_utils/car.h"


bool CarTracker::initialize() {
    car = datamanager()->writeChannel<sensor_utils::Car>(this,"CAR");
    firstRun = true;
    deltaTime = 0;

    /* state definition:
     * x        global x-position
     * y        global y-position
     * theta    global orientation
     * v        velocity in x-direction
     * a        acceleration in x-direction
     * omega    turn rate around z-axis
     */

    x.setZero();
    ukf.init(x);

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

   /************************
    * UKF start ************
    ************************/

    // define control input
    u.dx() = 0;
    u.dy() = 0;
    u.dtheta() = 0;
    u.dt() = 0.01; // time since UKF was last called (parameter, masked as control input)

    // simulate system
    deltaTime = lms::extra::PrecisionTime::since(last).toFloat<std::milli>()/1000;
    x = sys.f(x, u);

    // predict state for current time-step using the ukf
    x = ukf.predict(sys, u);
    //State<T> x_ukf = ukf.predict(sys, u);

    // actual measurement (from IMU/mouse/...)
    Measurement<T> measurementActual;
    measurementActual.ax() = 1;
    measurementActual.ay() = 1;
    measurementActual.omega() = 1;

    // perform measurement update
    x = ukf.update(mm, measurementActual);
    //x_ukf = ukf.update(mm, measurementActual);

    /************************
     * UKF end **************
     ************************/



    last = lms::extra::PrecisionTime::now();
    //logger.debug("cycle: ")<<"Delta t: "<< deltaTime;

    //lms::math::vertex2f pos;

    //car->updatePosition(pos,lms::math::vertex2f(cos(angle),sin(angle)));
    //car->updateVelocity(car->targetSpeed(),lms::math::vertex2f(cos(angle),sin(angle)));


    return true;
}

//void CarTracker::getFromVehicle(DeltaState &d){
//    //get needed values
//    float velocity = car->velocity();
//    float steeringFront = car->steeringFront();
//    float steeringRear = car->steeringRear();

//    float distance = velocity*deltaTime;
//    float radstand = getConfig()->get<float>("radstand",0.26);

//    //Man geht davon aus, dass die x,y-Abweichung, wegen kleiner Schritte, durch die Vorderräder gering ist.
//    d.x = distance*cos(steeringRear);
//    d.y = distance*sin(steeringRear);
//    d.phi = distance/radstand*sin(steeringFront-steeringRear)/cos(steeringRear);

//    /*
//    //Betrachte beide Achsen getrennt. Der Abstand zwischen den Rädern ist somit in einem Schritt nicht konstant!
//    lms::math::vertex2f rear(distance*cos(steeringRear),distance*sin(steeringRear));
//    lms::math::vertex2f front(distance*cos(steeringFront)+radstand,distance*sin(steeringFront));;
//    lms::math::vertex2f delta = front-rear;
//    //Man mittelt den Wert mit der Vorderachse (TODO macht das überhaupt sinn?)

//    d.x = rear.x;
//    d.y = rear.y;
//    d.phi = delta.angle();
//    */


//}

//void CarTracker::getFromTrajectory(DeltaState &d){
//    (void)d;
//    //TODO
//}

//void CarTracker::getFromMouseSensors(DeltaState &d){
//    (void)d;
//    //TODO
//}
//void CarTracker::getFromImu(DeltaState &d){
//    (void)d;
//    //TODO
//}

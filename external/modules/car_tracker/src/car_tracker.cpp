#include <cmath>
#include <algorithm>
#include "car_tracker.h"
#include "sensor_utils/car.h"


bool CarTracker::initialize() {
    car = writeChannel<sensor_utils::Car>("CAR");
    firstRun = true;   

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

void CarTracker::updateFromVehicle(){
    float dt=lms::extra::PrecisionTime::since(last).toFloat();
    float velocity = car->velocity();
    float steeringFront = car->steeringFront();
    float steeringRear = car->steeringRear();

    float distance = velocity*dt;
    float radstand = config().get<float>("radstand",0.26);


    //Man geht davon aus, dass die x,y-Abweichung, wegen kleiner Schritte, durch die Vorderräder gering ist.
    float dx = distance*cos(steeringRear);
    float dy = distance*sin(steeringRear);
    float dphi = distance/radstand*sin(steeringFront-steeringRear)/cos(steeringRear);

    //set values for UKF
    u.dx() = dx;
    u.dy() = dy;
    u.dtheta() = dphi;

    //TODO remove code after using u
    float angle = car->viewDirection().angle()+dphi;
    lms::math::vertex2f pos;
    //rotate in absolute position
    pos.x = dx;
    pos.y = dy;
    pos = pos.rotate(-angle);
    pos += car->position();
    car->updatePosition(pos,lms::math::vertex2f(cos(angle),sin(angle)));
    car->updateVelocity(car->targetSpeed(),lms::math::vertex2f(cos(angle),sin(angle)));
}

bool CarTracker::cycle() {
    if(firstRun){
        last = lms::extra::PrecisionTime::now();
        firstRun = false;
        return true;
    }

    if(config().get<bool>("updateFromVehicle",true)){
        updateFromVehicle();
        return true; //TODO remove after setting values using UKF
    }else{
        //TODO set u from ego-motion
        // define control input
        u.dx() = 0;
        u.dy() = 0;
        u.dtheta() = 0;
    }

   /************************
    * UKF start ************
    ************************/
    u.dt() = lms::extra::PrecisionTime::since(last).toFloat(); // time since UKF was last called (parameter, masked as control input)

    // simulate system
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

    //TODO update car velocity/pos etc.


    last = lms::extra::PrecisionTime::now();
    //logger.debug("cycle: ")<<"Delta t: "<< deltaTime;

    //lms::math::vertex2f pos;

    //car->updatePosition(pos,lms::math::vertex2f(cos(angle),sin(angle)));
    //car->updateVelocity(car->targetSpeed(),lms::math::vertex2f(cos(angle),sin(angle)));


    return true;
}

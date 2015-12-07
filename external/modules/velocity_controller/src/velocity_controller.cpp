#include "velocity_controller.h"
#include "lms/datamanager.h"
#include "lms/messaging.h"
#include <string>     // std::string, std::stof

bool VelocityController::initialize() {
    road = readChannel<street_environment::RoadLane>("MIDDLE_LANE");
    car = writeChannel<sensor_utils::Car>("CAR");
    lastCall = lms::extra::PrecisionTime::now()-lms::extra::PrecisionTime::fromMillis(config().get<float>("maxDeltaTInMs")*10);
    driving = false;
    return true;
}

bool VelocityController::deinitialize() {
    return true;
}

bool VelocityController::cycle() {
    //check for error-stop
    if(messaging()->receive("STOP_CAR").size() > 0){
        driving = false;
        logger.warn("cycle")<<"STOP_CAR called";
        return true;
    }
    for(std::string content : messaging()->receive("RC_STATE_CHANGED")){
        std::istringstream is(content);
        bool rc_on;
        is >> std::boolalpha >> rc_on;
        if(rc_on){
            driving = false;
        }else{
            driving = true;
        }
        logger.info("cycle")<<"RC_STATE_CHANGED";
        return true;
    }
    //TODO
    driving = true;
    if(!driving){
        sensor_utils::Car::State s;
        s.state = sensor_utils::Car::StateType::IDLE;
        s.targetSpeed = 0;
        s.steering_front = 0;
        s.steering_rear = 0;
        s.priority = 100;
        s.name = "DRIVING_DISABLED";
        car->putState(s);
        logger.debug("error")<<"driving disabled";
        return true;
    }else{
        car->removeState("DRIVING_DISABLED");
    }
    sensor_utils::Car::State *tmp = car->getState("DEFAULT");
    sensor_utils::Car::State s;
    if(tmp){
        s = *tmp;
    }
    s.state = sensor_utils::Car::StateType::DRIVING;
    s.targetSpeed = 0;
    s.priority = 10;
    s.name = "DEFAULT";
    float vel = INFINITY;
    for (const std::string &m : messaging()->receive("CAR_VELOCITY")){
        float tmpVel = stof(m);
        if(fabs(tmpVel) < vel){
            vel = tmpVel;
        }
    }
    if(vel != INFINITY){
        logger.debug("setting speed manually: ") << vel;
        s.targetSpeed = vel;
    }

    if(!defaultDrive(s))
        return true;

    if(config().get<bool>("launchControllEnabled",false)){
        launchControll(car->targetSpeed(),car->velocity(),s);
    }

    car->putState(s);

    logger.debug("info") << "end-velocity: " << car->targetSpeed();
    lastCall = lms::extra::PrecisionTime::now();
    return true;
}

bool VelocityController::defaultDrive(sensor_utils::Car::State &state){
    //TODO check if road is invalid

    float maxSpeed = config().get<float>("maxSpeed",1);
    float minCurveSpeed = config().get<float>("minCurveSpeed",maxSpeed/2);
    float maxCurvation = config().get<float>("maxCurvation",1);
    int partsNeeded = config().get<float>("forcastLength",1)/road->polarPartLength;
    if(partsNeeded > ((int)road->polarDarstellung.size())-2){
        logger.warn("cycle")<<"not enough parts available: " << partsNeeded;
        partsNeeded = ((int)road->polarDarstellung.size())-2;
    }
    logger.debug("defaultDrive") << "parts needed: " << partsNeeded;
    if(partsNeeded == 0 || partsNeeded == INFINITY || partsNeeded == NAN){
        logger.warn("cycle")<<"partsNeeded not valid: " << partsNeeded;
        return false;
    }

    //TODO gewichteter mittelwert!
    //TODO oder median benutzen
    //TODO der ansatz ist prinzipiell bei S-Kurven fragwürdig!
    float middleCurvation = 0;
    for(int i = 0; i < partsNeeded; i++){
        middleCurvation += road->polarDarstellung[i+2];
    }
    middleCurvation = fabs(middleCurvation)/partsNeeded;
    logger.debug("defaultDrive") <<"middle-curcation: " << middleCurvation;
    float velocity = (minCurveSpeed-maxSpeed)/(maxCurvation)*(middleCurvation)+maxSpeed;
    logger.debug("defaultDrive")<<"velocity: "<<velocity;
    state.targetSpeed = velocity;
    return true;
}


bool VelocityController::launchControll(float newVelocity,float currentVelocity,sensor_utils::Car::State &state){
    float deltaT = lms::extra::PrecisionTime::since(lastCall).toFloat();
    //beim Start wird das z.B. angenommen
    if(deltaT*1000 > config().get<float>("maxDeltaTInMs",100)){
        deltaT = config().get<int>("defaultDeltaTInMs",10)*1000;
    }
    float acc = (newVelocity -currentVelocity)/deltaT;
    float velocityLimited = newVelocity;
    if(acc > config().get<float>("maxAcc",1)){
        velocityLimited = currentVelocity+config().get<float>("maxAcc")*deltaT;
    }else if(acc < config().get<float>("minAcc",-1)){
        velocityLimited = currentVelocity+config().get<float>("minAcc")*deltaT;
    }else{
        return false;
    }
    state.targetSpeed = velocityLimited;
    return true;
}


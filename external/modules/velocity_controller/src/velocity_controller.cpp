#include "velocity_controller.h"
#include "lms/datamanager.h"
#include "lms/messaging.h"
#include <string>     // std::string, std::stof

bool VelocityController::initialize() {
    road = datamanager()->readChannel<street_environment::RoadLane>(this,"MIDDLE_LANE");
    car = datamanager()->writeChannel<sensor_utils::Car>(this,"CAR");
    config = getConfig();
    lastCall = lms::extra::PrecisionTime::now()-lms::extra::PrecisionTime::fromMillis(config->get<float>("maxDeltaTInMs")*10);
    return true;
}

bool VelocityController::deinitialize() {
    return true;
}

bool VelocityController::cycle() {
    //check for error-stop
    if(messaging()->receive("STOP_CAR").size() > 0){
        car->targetSpeed = 0;
        logger.warn("cycle")<<"STOP_CAR called";
        return true;
    }
    float vel = INFINITY;
    for (const std::string &m : messaging()->receive("CAR_VELOCITY")){
        float tmpVel = stof(m);
        if(fabs(tmpVel) < vel){
            vel = tmpVel;
        }
    }
    if(vel != INFINITY){
        logger.debug("setting speed manually: ") << vel;
        car->targetSpeed = vel;
    }

    if(!defaultDrive())
        return true;
    if(config->get<bool>("launchControllEnabled",true)){
        launchControll(car->targetSpeed,car->velocity());
    }
    logger.debug("info") << "end-velocity: " << car->targetSpeed;
    lastCall = lms::extra::PrecisionTime::now();
    return true;
}

bool VelocityController::defaultDrive(){
    //TODO check if road is invalid

    float maxSpeed = config->get<float>("maxSpeed",1);
    float minCurveSpeed = config->get<float>("minCurveSpeed",maxSpeed/2);
    float maxCurvation = config->get<float>("maxCurvation",1);
    int partsNeeded = config->get<float>("forcastLength",1)/road->polarPartLength;
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
    car->targetSpeed = velocity;
    return true;
}


bool VelocityController::launchControll(float newVelocity,float currentVelocity){
    float deltaT = lms::extra::PrecisionTime::since(lastCall).toFloat();
    //beim Start wird das z.B. angenommen
    if(deltaT*1000 > config->get<float>("maxDeltaTInMs",100)){
        deltaT = config->get<int>("defaultDeltaTInMs",10)*1000;
    }
    float acc = (newVelocity -currentVelocity)/deltaT;
    float velocityLimited = newVelocity;
    if(acc > config->get<float>("maxAcc",1)){
        velocityLimited = currentVelocity+config->get<float>("maxAcc")*deltaT;
    }else if(acc < config->get<float>("minAcc",-1)){
        velocityLimited = currentVelocity+config->get<float>("minAcc")*deltaT;
    }else{
        return false;
    }
    car->targetSpeed = velocityLimited;
    return true;
}


#include "velocity_controller.h"
#include "lms/datamanager.h"

bool VelocityController::initialize() {
    middle = datamanager()->readChannel<Environment::RoadLane>(this,"MIDDLE_LANE");
    config = getConfig();
    return true;
}

bool VelocityController::deinitialize() {
    return true;
}

bool VelocityController::cycle() {

    float maxSpeed = config->get<float>("maxSpeed",1);
    float minCurveSpeed = config->get<float>("minCurveSpeed",maxSpeed/2);
    float maxCurvationInRad = config->get<float>("maxCurvationInRad",M_PI/4);
    int partsNeeded = config->get<float>("forcastLength",1)/middle->polarPartLength;
    if(partsNeeded > (int)middle->points().size()){
        partsNeeded = middle->points().size();
    }
    if(partsNeeded == 0 || partsNeeded == INFINITY || partsNeeded == NAN){
        logger.warn("cycle")<<"parsNeeded not valid: " << partsNeeded;
    }

    //TODO gewichteter mittelwert!
    //TODO der ansatz ist prinzipiell bei S-Kurven schlecht!
    float middleCurvation = 0;
    for(int i = 0; i < partsNeeded; i++){
        middleCurvation += middle->polarDarstellung[i+1];
    }
    middleCurvation = fabs(middleCurvation)/partsNeeded;
    float velocity = (minCurveSpeed-maxSpeed)/(maxCurvationInRad)*(middleCurvation)+maxSpeed;
    controlData->control.velocity.velocity = velocity;
    return true;
}


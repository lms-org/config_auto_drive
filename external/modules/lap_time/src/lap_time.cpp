#include "lap_time.h"
bool LapTime::initialize() {
    environment = readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT");
    car = readChannel<street_environment::Car>("CAR");
    start = lms::Time::ZERO;
    crossingStartLine = false;
    distanceDriven = 0;
    maxSpeed = 0;
    minSpeed = 1000;
    return true;
}

bool LapTime::deinitialize() {
    return true;
}

bool LapTime::cycle() {
    //search for a startLine
    for(street_environment::EnvironmentObjectPtr ptr:environment->objects){
        if(ptr->getType() == street_environment::StartLine::TYPE){
            street_environment::StartLinePtr startLine = std::static_pointer_cast<street_environment::StartLine>(ptr);
            if(startLine->distanceTang() < 0){
                if(!crossingStartLine){
                    crossedStartLine();
                }
                crossingStartLine = true;
                return true;
            }
        }
    }
    //reset it
    crossingStartLine = false;
    distanceDriven += car->deltaPosition().length();
    if(car->velocity() > maxSpeed){
        maxSpeed = car->velocity();
    }
    if(car->velocity() < minSpeed){
        minSpeed = car->velocity();
    }

    return true;
}

void LapTime::crossedStartLine(){
    if(start != lms::Time::ZERO){
        lms::Time lapTime = start.since();
        logger.info("crossedStartLine")<<"Lap-time: "<< lapTime.toFloat() <<"s";
        logger.info("crossedStartLine")<<"Distance driven: "<< distanceDriven <<"n";
        logger.info("crossedStartLine")<<"Mean velocity: "<< distanceDriven/lapTime.toFloat() <<"m/s";
        logger.info("crossedStartLine")<<"Min velocity: "<< minSpeed <<"m/s";
        logger.info("crossedStartLine")<<"Max velocity: "<< maxSpeed <<"m/s";

    }else{
        logger.info("startRace");
    }
    start = lms::Time::now();
    distanceDriven = 0;
}

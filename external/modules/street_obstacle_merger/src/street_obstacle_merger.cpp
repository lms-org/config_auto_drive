#include "street_obstacle_merger.h"
#include "lms/datamanager.h"
#include "street_environment/obstacle.h"

bool StreetObstacleMerger::initialize() {
    envInput = datamanager()->readChannel<street_environment::EnvironmentObjects>(this,"ENVIRONMENT_INPUT");
    envOutput = datamanager()->writeChannel<street_environment::EnvironmentObjects>(this,"ENVIRONMENT_OUTPUT");

    //We should havet the roadlane and the car from the current cycle
    car = datamanager()->writeChannel<sensor_utils::Car>(this,"CAR");
    middle = datamanager()->writeChannel<street_environment::RoadLane>(this,"MIDDLE_LANE");
    return true;
}

bool StreetObstacleMerger::deinitialize() {
    return true;
}

bool StreetObstacleMerger::cycle() {
    //get vectors from environments
    street_environment::EnvironmentObstacles obstaclesNew;
    street_environment::EnvironmentObstacles obstaclesOld;


    getObstacles(*envInput,obstaclesNew);
    getObstacles(*envOutput,obstaclesOld);

    logger.debug("cycle")<<"number of old obstacles" << obstaclesOld.objects.size();

    //update old obstacles
    for(std::shared_ptr<street_environment::Obstacle> &obst:obstaclesOld.objects){
        obst->kalman(*middle,car->movedDistance());
    }

    //kalman new obstacles
    for(std::shared_ptr<street_environment::Obstacle> &obst:obstaclesNew.objects){
        obst->kalman(*middle,0);
    }

    logger.debug("cycle")<<"number of new obstacles (before merge)" << obstaclesNew.objects.size();
    //merge them
    merge(obstaclesNew,obstaclesOld);
    logger.debug("cycle")<<"number of obstacles (after merge)" << obstaclesOld.objects.size();

    //Remove invalid obstacles
    filter(obstaclesOld);
    logger.debug("cycle")<<"number of obstacles (after filter)" << obstaclesOld.objects.size();
    //create new env output
    createOutput(obstaclesOld);
    logger.debug("cycle")<<"number of obstacles (output)" << envOutput->objects.size();

    return true;
}

void StreetObstacleMerger::createOutput(street_environment::EnvironmentObstacles &obstaclesOld){
    //clear old obstacles, if I didn't failed those shared pointers it shouldn't cause an segfault :)

    envOutput->objects.clear();
    for(uint i = 0; i < obstaclesOld.objects.size(); i++){
        //create a copy
        //std::shared_ptr<street_environment::Obstacle> obstacle(new street_environment::Obstacle(*obstaclesOld[i]));
        envOutput->objects.push_back(obstaclesOld.objects[i]);
    }
}

void StreetObstacleMerger::filter(street_environment::EnvironmentObstacles &obstaclesOld){
    //Decrease foundCounter
    for(uint i = 0; i < obstaclesOld.objects.size();i++){
        /*Not that smart :D
        if(obstaclesOld.objects[i]->timesFound()>10){
            obstaclesOld.objects[i]->found(10-obstaclesOld.objects[i]->timesFound());
        }
        */
        obstaclesOld.objects[i]->found(-1);
    }

    //TODO hier könnte man sich auch etwas besseres einfallen lassen
    for(uint i = 0; i < obstaclesOld.objects.size(); i++){
        logger.debug("objectpos: ")<<obstaclesOld.objects[i]->position().x<<" "<<obstaclesOld.objects[i]->position().y;
        if(obstaclesOld.objects[i]->timesFound() <= 0 || obstaclesOld.objects[i]->position().x < -0.35){
            obstaclesOld.objects.erase(obstaclesOld.objects.begin() + i);
        }
    }
}


void StreetObstacleMerger::merge(street_environment::EnvironmentObstacles &obstaclesNew,street_environment::EnvironmentObstacles &obstaclesOld){
    //check if obstacles can be merged
    for(uint k = 0; k < obstaclesNew.objects.size();k++){
        bool merged = false;
        for(uint i = 0; i < obstaclesOld.objects.size();i++){
            merged = merge(obstaclesOld.objects[i],obstaclesNew.objects[k]);
            if(merged){
                //TODO create merged object
                //TODO increase some "times validated counter"
                obstaclesOld.objects[i]->found(2); //+2 as we remove one in filter
                break;
            }
        }
        if(!merged){
            //Basic value
            obstaclesNew.objects[k]->found(5);
            obstaclesOld.objects.push_back(obstaclesNew.objects[k]);
        }
    }
}

void StreetObstacleMerger::getObstacles(const street_environment::EnvironmentObjects &env,street_environment::EnvironmentObstacles &output){
    for(const std::shared_ptr<street_environment::EnvironmentObject> obj : env.objects){
        if(obj->getType() == 1){
            //that cast ignores, that the obj was const
            std::shared_ptr<street_environment::Obstacle> obst = std::static_pointer_cast<street_environment::Obstacle>(obj);
            output.objects.push_back(obst);
        }
    }
}

bool StreetObstacleMerger::merge(const std::shared_ptr<street_environment::Obstacle> &from,const std::shared_ptr<street_environment::Obstacle> &to){
    //TODO, tangentialen und orthogonalen abstand zur mittellinie benutzen!
    if(from->position().distance(to->position())<0.5){
        return true;
    }
    return false;
}

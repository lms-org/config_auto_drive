#include "street_obstacle_merger.h"
#include "lms/datamanager.h"
#include "street_environment/obstacle.h"
#include "street_environment/crossing.h"

bool StreetObjectMerger::initialize() {
    envInput = readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_INPUT");
    envOutput = writeChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_OUTPUT");

    //We should have the roadlane and the car from the current cycle
    car = readChannel<sensor_utils::Car>("CAR");
    middle = readChannel<street_environment::RoadLane>("MIDDLE_LANE");

    if(config().hasKey("visibleAreas")){
        std::vector<std::string> sRects = config().getArray<std::string>("visibleAreas");
        for(std::string &sRect: sRects){
            lms::math::Rect r;
            if(!config().hasKey(sRect+"_x") || !config().hasKey(sRect+"_y") ||!config().hasKey(sRect+"_w")||!config().hasKey(sRect+"_h")){
                logger.error("initialize")<<"visibleArea not valid: "<< sRect;
                return false;
            }
            r.x = config().get<float>(sRect+"_x");
            r.y = config().get<float>(sRect+"_y");
            r.width = config().get<float>(sRect+"_w");
            r.height = config().get<float>(sRect+"_h");
            visibleAreas.push_back(r);
        }
    }
    return true;
}

bool StreetObjectMerger::inVisibleArea(float x, float y){
    for(lms::math::Rect& r:visibleAreas){
        if(r.contains(x,y))
            return true;
    }
    return false;
}

bool StreetObjectMerger::deinitialize() {
    return true;
}

bool StreetObjectMerger::cycle() {

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

void StreetObjectMerger::createOutput(street_environment::EnvironmentObstacles &obstaclesOld){
    //clear old obstacles
    envOutput->objects.clear();
    for(uint i = 0; i < obstaclesOld.objects.size(); i++){
        envOutput->objects.push_back(obstaclesOld.objects[i]);
    }
}

void StreetObjectMerger::filter(street_environment::EnvironmentObstacles &obstacles){
    //Decrease foundCounter
    for(uint i = 0; i < obstacles.objects.size();i++){
        if(!inVisibleArea(obstacles.objects[i]->position().x,
                obstacles.objects[i]->position().y)){
            logger.error("OBJTECT CANT BE FOUND")<<obstacles.objects[i]->position().x<<" "<<
                    obstacles.objects[i]->position().y;
            //if the obstacle can't be found by the sensors we won't decrease the counter, we just translate it every step and remove it later on
            continue;
        }
        //Not that smart :D
        /*
        if(obstacles.objects[i]->trust()>10){
            obstacles.objects[i]->trustIt(10);
        }
        */
        obstacles.objects[i]->trustIt(-2);//TODO magic number
    }

    //TODO hier könnte man sich auch etwas besseres einfallen lassen
    for(uint i = 0; i < obstacles.objects.size(); i++){
        logger.debug("objectpos: ")<<obstacles.objects[i]->position().x<<" "<<obstacles.objects[i]->position().y;
        if(obstacles.objects[i]->trust() <= 0 || obstacles.objects[i]->position().x < -0.35){
            obstacles.objects.erase(obstacles.objects.begin() + i);
        }
    }
}


void StreetObjectMerger::merge(street_environment::EnvironmentObstacles &obstaclesNew,street_environment::EnvironmentObstacles &obstaclesOld){
    //check if obstacles can be merged
    for(uint k = 0; k < obstaclesNew.objects.size();k++){
        bool merged = false;
        for(uint i = 0; i < obstaclesOld.objects.size();i++){
            merged = obstaclesOld.objects[i]->match(*obstaclesNew.objects[k].get());
            if(merged){
                lms::math::vertex2f pos = obstaclesNew.objects[k]->position()+obstaclesOld.objects[i]->position();
                pos = pos*0.5;
                obstaclesOld.objects[i]->updatePosition(pos);
                //TODO create merged object
                //TODO increase some "times validated counter"
                obstaclesOld.objects[i]->trustIt(obstaclesNew.objects[k]->trust());
                break;
            }
        }
        if(!merged){
            //Basic value
            obstaclesOld.objects.push_back(obstaclesNew.objects[k]);
        }
    }
}

void StreetObjectMerger::getObstacles(const street_environment::EnvironmentObjects &env,street_environment::EnvironmentObstacles &output){
    for(const std::shared_ptr<street_environment::EnvironmentObject> obj : env.objects){
        if(obj->getType() == street_environment::Obstacle::TYPE){
            //that cast ignores, that the obj was const
            std::shared_ptr<street_environment::Obstacle> obst = std::static_pointer_cast<street_environment::Obstacle>(obj);
            output.objects.push_back(obst); //could that cause a segfault?
        }else if(obj->getType() == street_environment::Crossing::TYPE){
            //TODO
            //std::shared_ptr<street_environment::Obstacle> obst = std::static_pointer_cast<street_environment::Crossing>(obj);
            //output.objects.push_back(obst);
        }
    }
}

#include "street_obstacle_merger.h"
#include "street_environment/obstacle.h"
#include "street_environment/crossing.h"
#include "street_environment/start_line.h"
#include "area_of_detection/area_of_detection.h"
#include "phoenix_CC2016_service/phoenix_CC2016_service.h"

bool StreetObjectMerger::initialize() {
    envInput = readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_INPUT");
    envOutput = writeChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_OUTPUT");

    //We should have the roadlane and the car from the current cycle
    car = readChannel<sensor_utils::Car>("CAR");
    middle = readChannel<street_environment::RoadLane>("MIDDLE_LANE");
    //hack for drawing
    visibleAreas_hack = writeChannel<std::vector<lms::math::Rect>>("VISIBLE_AREAS");
    return true;
}

bool StreetObjectMerger::inVisibleArea(float x, float y){
    std::vector<lms::math::Rect> visibleAreas = getService<area_of_detection::AreaOfDetection>("AreaOfDetection")->visibleAreas();
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
    //reset obstacles
    if(getService<phoenix_CC2016_service::Phoenix_CC2016Service>("PHOENIX_SERVICE")->rcStateChanged()){
        envOutput->objects.clear();
    }

    *visibleAreas_hack = getService<area_of_detection::AreaOfDetection>("AreaOfDetection")->visibleAreas();

    //get vectors from environments
    street_environment::EnvironmentObstacles obstaclesNew;
    street_environment::EnvironmentObstacles obstaclesOld;


    getObstacles(*envInput,obstaclesNew);
    getObstacles(*envOutput,obstaclesOld);

    logger.debug("cycle")<<"number of new obstacles" << obstaclesNew.objects.size();
    logger.debug("cycle")<<"number of old obstacles" << obstaclesOld.objects.size();

    //update old obstacles
    for(std::shared_ptr<street_environment::Obstacle> &obst:obstaclesOld.objects){
        obst->kalman(*middle,car->movedDistance());
    }

    //kalman new obstacles
    for(std::shared_ptr<street_environment::Obstacle> &obst:obstaclesNew.objects){
        obst->kalman(*middle,0);
        logger.debug("TRUST START")<<obst->trust();
    }

    //merge new obstacles
    for(int i = 0; i < (int)obstaclesNew.objects.size(); i++){
        std::shared_ptr<street_environment::Obstacle>  obst1 = obstaclesNew.objects[i];
        int findCound = 0;
        lms::math::vertex2f mergePos = obst1->position();
        float orthMinRes = obst1->distanceOrth()-obst1->width()/2;
        float orthMaxRes = obst1->distanceOrth()+obst1->width()/2;
        for(int k = i+1; k < (int)obstaclesNew.objects.size(); ){
            std::shared_ptr<street_environment::Obstacle>  obst2 = obstaclesNew.objects[k];
            if(obst1->match(*obst2)){
                findCound++;
                mergePos +=  obst2->position();
                float orthMin = obst2->distanceOrth()-obst2->width()/2;
                if(orthMin < orthMinRes){
                    orthMinRes = orthMin;
                }
                float orthMax = obst2->distanceOrth()+obst2->width()/2;
                if(orthMax > orthMaxRes){
                    orthMaxRes = orthMax;
                }
                //remove it
                obstaclesNew.objects.erase(obstaclesNew.objects.begin()+k);
            }else{
                k++;
            }
        }
        if(findCound > 0){
            mergePos=mergePos/findCound;
            obst1->width(orthMaxRes-orthMinRes);
        }
    }

    //kalman merged new obstacles
    for(std::shared_ptr<street_environment::Obstacle> &obst:obstaclesNew.objects){
        obst->kalman(*middle,0);
        //calculate the trust
    }

    logger.debug("cycle")<<"number of new obstacles (before merge)" << obstaclesNew.objects.size();
    //merge them
    merge(obstaclesNew,obstaclesOld);
    logger.debug("cycle")<<"number of obstacles (after merge)" << obstaclesOld.objects.size();

    //kalman obstacles
    for(std::shared_ptr<street_environment::Obstacle> &obst:obstaclesOld.objects){
        obst->kalman(*middle,0);
        if(fabs(obst->distanceOrth()) >0.3 && obst->trust() > 0.1){
            obst->setTrust(0.1);
        }
        if(fabs(obst->distanceOrth()) > 0.4){
            obst->setTrust(0);
        }
        if(fabs(obst->distanceTang()) > 2){
            obst->setTrust(0);
        }
    }

    //create new env output
    createOutput(obstaclesOld);
    logger.debug("cycle")<<"number of obstacles (output)" << envOutput->objects.size();

    return true;
}

void StreetObjectMerger::createOutput(street_environment::EnvironmentObstacles &obstaclesOld){
    //clear old obstacles
    envOutput->objects.clear();
    for(uint i = 0; i < obstaclesOld.objects.size(); i++){
        if(obstaclesOld.objects[i]->trust() > 0){
            logger.debug("keep it")<<obstaclesOld.objects[i]->trust();
            envOutput->objects.push_back(obstaclesOld.objects[i]);
        }
    }
}


void StreetObjectMerger::merge(street_environment::EnvironmentObstacles &obstaclesNew,street_environment::EnvironmentObstacles &obstaclesOld){
    std::vector<int> verifiedOld;
    int oldSize = obstaclesOld.objects.size();
    //check if obstacles can be merged
    for(street_environment::ObstaclePtr obstNew :  obstaclesNew.objects){
        bool merged = false;
        for(int i = 0; i < (int)obstaclesOld.objects.size(); i++){
            street_environment::ObstaclePtr obstOld = obstaclesOld.objects[i];
            if(obstNew->getType() != obstOld->getType()){
                continue;
            }
            merged = obstOld->match(*obstNew);
            if(merged){
                obstOld->updatePosition(obstNew->position());
                obstOld->viewDirection(obstNew->viewDirection());//ok
                obstOld->width(obstNew->width());
                float newTrust = obstOld->trust() + obstNew->trust();
                if(newTrust < 0)
                    newTrust = 0;
                else if(newTrust > 1)
                    newTrust = 1;
                obstOld->setTrust(newTrust);
                verifiedOld.push_back(i);
                break;
            }
        }
        if(!merged){
            //Basic value
            obstaclesOld.objects.push_back(obstNew);
        }
    }
    //Decrease trust in obstacles that weren't found
    for(int i = 0; i < oldSize;i++){
        if(std::find(verifiedOld.begin(), verifiedOld.end(), i) == verifiedOld.end()){
           if(!inVisibleArea(obstaclesOld.objects[i]->position().x,
                        obstaclesOld.objects[i]->position().y))
               continue;//obstacle can't be found

            //old obstacle wasn't found
            float dt = obstaclesOld.objects[i]->getDeltaTrust();
            if(dt < -config().get<float>("trustReducer",0.1)){
                dt *= 2;
            }else{
                dt = -config().get<float>("trustReducer",0.1);
            }
            float newTrust = obstaclesOld.objects[i]->trust()+dt;
            if(newTrust < 0)
                newTrust = 0;
            else if(newTrust > 1)//should not happen
                newTrust = 1;
             obstaclesOld.objects[i]->setTrust(newTrust);
        }
    }
}

void StreetObjectMerger::getObstacles(const street_environment::EnvironmentObjects &env,street_environment::EnvironmentObstacles &output){
    for(const std::shared_ptr<street_environment::EnvironmentObject> obj : env.objects){
        if(obj->getType() == street_environment::Obstacle::TYPE){
            //that cast ignores, that the obj was const
            std::shared_ptr<street_environment::Obstacle> obst = std::static_pointer_cast<street_environment::Obstacle>(obj);
            output.objects.push_back(obst);
        }else if(obj->getType() == street_environment::Crossing::TYPE){
            std::shared_ptr<street_environment::Obstacle> crossing = std::static_pointer_cast<street_environment::Crossing>(obj);
            output.objects.push_back(crossing);
        }else if(obj->getType() == street_environment::StartLine::TYPE){
            std::shared_ptr<street_environment::StartLine> startLine = std::static_pointer_cast<street_environment::StartLine>(obj);
            output.objects.push_back(startLine);
        }
    }
}

#include "street_obstacle_merger.h"
#include "street_environment/obstacle.h"
#include "street_environment/crossing.h"
#include "street_environment/start_line.h"
#include "area_of_detection/area_of_detection.h"
#include "phoenix_CC2016_service/phoenix_CC2016_service.h"

bool StreetObjectMaster::initialize() {
    envOutput = writeChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_OUTPUT");

    std::vector<std::string> envInputs = config().getArray<std::string>("new_environments");
    for(const std::string &s:envInputs){
        envInput.push_back(readChannel<street_environment::EnvironmentObjects>(s));
    }

    //We should have the roadlane and the car from the current cycle
    poseHistory = readChannel<lms::math::Pose2DHistory>("POSE2D_HISTORY");
    middle = readChannel<street_environment::RoadLane>("MIDDLE_LANE");

    visibleAreasToDraw = writeChannel<std::vector<lms::math::Rect>>("VISIBLE_AREAS");
    return true;
}

bool StreetObjectMaster::inVisibleArea(float x, float y){
    std::vector<lms::math::Rect> visibleAreas = getService<area_of_detection::AreaOfDetection>("AreaOfDetection")->visibleAreas();
    for(lms::math::Rect& r:visibleAreas){
        if(r.contains(x,y))
            return true;
    }
    return false;
}

bool StreetObjectMaster::deinitialize() {
    return true;
}

bool StreetObjectMaster::cycle() {
    //reset obstacles TODO HACK, not nice at all
    if(getService<phoenix_CC2016_service::Phoenix_CC2016Service>("PHOENIX_SERVICE")->rcStateChanged()){
        envOutput->objects.clear();
    }

    *visibleAreasToDraw = getService<area_of_detection::AreaOfDetection>("AreaOfDetection")->visibleAreas();

    //get vectors from environments
    street_environment::EnvironmentObstacles obstaclesNew;
    street_environment::EnvironmentObstacles obstaclesOld;

    //collect all new found obstacles
    for(lms::ReadDataChannel<street_environment::EnvironmentObjects> &envO: envInput){
        getObstacles(*envO,obstaclesNew);
    }
    getObstacles(*envOutput,obstaclesOld);
    lms::math::Pose2D dPose = getDeltaPose();

    logger.debug("cycle")<<"number of new obstacles" << obstaclesNew.objects.size();
    logger.debug("cycle")<<"number of old obstacles" << obstaclesOld.objects.size();

    logger.debug("cycle")<<"translate old obstacles by: "<<dPose.x << " "<<dPose.y;
    //translate old old obstacles
    for(std::shared_ptr<street_environment::Obstacle> &obst:obstaclesOld.objects){
        obst->translate(dPose.x,dPose.y);
    }

    //merge new obstacles
    for(int i = 0; i < (int)obstaclesNew.objects.size(); i++){
        std::shared_ptr<street_environment::Obstacle>  obst1 = obstaclesNew.objects[i];
        for(int k = i+1; k < (int)obstaclesNew.objects.size(); ){
            std::shared_ptr<street_environment::Obstacle>  obst2 = obstaclesNew.objects[k];
            //TODO check if their bounding box
            if(obst1->match(*obst2)){
                //merge the points
                obst1->addPoints(obst2->points());
                obstaclesNew.objects.erase(obstaclesNew.objects.begin()+k);
            }else{
                k++;
            }
        }
    }
    logger.debug("cycle")<<"number of new obstacles (before merge)" << obstaclesNew.objects.size();
    //merge them
    merge(obstaclesNew,obstaclesOld);
    logger.debug("cycle")<<"number of obstacles (after merge)" << obstaclesOld.objects.size();

    //kalman obstacles
    for(std::shared_ptr<street_environment::Obstacle> &obst:obstaclesOld.objects){
        //TODO obst->kalman(*middle,0);
        if(fabs(distanceOrth(obst)) >0.4 && obst->trust() > 0.1){
            obst->setTrust(0.1);
        }
        if(fabs(distanceOrth(obst)) > 0.6){
            obst->setTrust(0);
        }
        if(fabs(distanceTang(obst)) > 2){
            obst->setTrust(0);
        }
        if(obst->getType() == street_environment::Crossing::TYPE || obst->getType() == street_environment::StartLine::TYPE){
            checkAngle(obst);
            //check if a crossing is blocked
            if(obst->getType() == street_environment::Crossing::TYPE){
                std::shared_ptr<street_environment::Crossing> crossing = std::static_pointer_cast<street_environment::Crossing>(obst);
                crossing->blocked(false);
                lms::math::vertex2f crossingPos = crossing->position();
                lms::math::vertex2f crossingView = crossing->viewDirection().normalize();
                lms::math::Rect blockedRect;
                lms::math::vertex2f pos = crossingPos+crossingView*0.4;
                blockedRect.x = pos.x;
                blockedRect.y = pos.y;
                blockedRect.width = 1;
                blockedRect.height = 0.5;
                for(std::shared_ptr<street_environment::Obstacle> &obstBlock:obstaclesOld.objects){
                    if(obstBlock->TYPE == street_environment::Obstacle::TYPE){
                        if(blockedRect.contains(obstBlock->position().x,obstBlock->position().y)){
                            //TODO trust value
                            crossing->blocked(true);
                            break;
                        }
                    }
                }
            }

        }
    }


    //create new env output
    createOutput(obstaclesOld);
    logger.debug("cycle")<<"number of obstacles (output)" << envOutput->objects.size();

    return true;
}

void StreetObjectMaster::createOutput(street_environment::EnvironmentObstacles &obstaclesOld){
    //clear old obstacles
    envOutput->objects.clear();
    for(uint i = 0; i < obstaclesOld.objects.size(); i++){
        if(obstaclesOld.objects[i]->trust() > 0){
            logger.debug("keep it")<<obstaclesOld.objects[i]->trust();
            envOutput->objects.push_back(obstaclesOld.objects[i]);
        }else{
            logger.debug("throwing away obstacle")<<obstaclesOld.objects[i]->trust();
        }
    }
}


void StreetObjectMaster::merge(street_environment::EnvironmentObstacles &obstaclesNew,street_environment::EnvironmentObstacles &obstaclesOld){
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
                if(obstOld->getType() == street_environment::Crossing::TYPE){
                    street_environment::CrossingPtr cptr = std::static_pointer_cast<street_environment::Crossing>(obstOld);
                    street_environment::CrossingPtr cptr2 = std::static_pointer_cast<street_environment::Crossing>(obstNew);
                    cptr->viewDirection(cptr2->viewDirection());
                    cptr->blocked(cptr2->blocked());
                }
                //TODO obstOld->updatePosition(obstNew->position());
                obstOld->clearPoints();
                obstOld->addPoint(obstNew->position());

                //TODO obstOld->viewDirection(obstNew->viewDirection());//ok
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

void StreetObjectMaster::getObstacles(const street_environment::EnvironmentObjects &env,street_environment::EnvironmentObstacles &output){
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

void StreetObjectMaster::checkAngle(street_environment::ObstaclePtr obst){
    float maxAngleBetweenCrossingAndRoad = config().get<float>("maxAngleBetweenCrossingAndRoad",0.4);
    float currentDis = 0;
    for(int i = 1; i < static_cast<int>(middle->points().size()); i++){
        lms::math::vertex2f streetDir = middle->points()[i]-middle->points()[i-1];
        currentDis += streetDir.length();
        if(currentDis > distanceTang(obst)){
            float deltaAngle = streetDir.angleBetween(obst->viewDirection());
            if(deltaAngle > maxAngleBetweenCrossingAndRoad){
                logger.info("crossing/startline not trusted")<<"crossingDistance: "<<distanceTang(obst)<<" "<< currentDis<<" roadPos: "<<i << "angle to road: "<< deltaAngle<< " roadAngle: "<<streetDir.angle() << " crossingViewDir "<<obst->viewDirection();
                obst->setTrust(0.0);
            }else{
                break;
            }
        }
    }
}

float StreetObjectMaster::distanceTang(street_environment::ObstaclePtr obstacle){
    float t,o;
    middle->firstOrthogonalDistance(obstacle->position(),o,t);
    return t;
}

float StreetObjectMaster::distanceOrth(street_environment::ObstaclePtr obstacle){
    float t,o;
    middle->firstOrthogonalDistance(obstacle->position(),o,t);
    return o;

}

lms::math::Pose2D StreetObjectMaster::getDeltaPose() {
    lms::math::Pose2D oldPose, deltaPose;
    if (poseHistory->getPose(lastUpdate.toFloat<std::milli, double>(),
                             oldPose)) {
        lms::math::CoordinateSystem2D coord(oldPose);
        deltaPose = coord.transformTo(poseHistory->currentPose());
    } else {
        logger.warn("cycle") << "no valid pose found: "
                             << lastUpdate.toFloat<std::milli, double>();
    }
    lastUpdate = lms::Time::now();
    return deltaPose;
}


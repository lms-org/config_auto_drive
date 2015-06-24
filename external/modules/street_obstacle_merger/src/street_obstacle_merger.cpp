#include "street_obstacle_merger.h"
#include "lms/datamanager.h"

bool StreetObstacleMerger::initialize() {
    envInput = datamanager()->readChannel<street_environment::Environment>(this,"ENVIRONMENT_INPUT");
    envOutput = datamanager()->writeChannel<street_environment::Environment>(this,"ENVIRONMENT_OUTPUT");
   return true;
}

bool StreetObstacleMerger::deinitialize() {
    return true;
}

bool StreetObstacleMerger::cycle() {
    //get vectors from environments
    std::vector<street_environment::Obstacle*> obstaclesNew;
    std::vector<street_environment::Obstacle*> obstaclesOld;

    getObstacles(*envInput,obstaclesNew);
    getObstacles(*envOutput,obstaclesOld);

    //TODO kalman stuff!

    //merge them
    merge(obstaclesNew,obstaclesOld);

    //Remove invalid obstacles
    filter(obstaclesOld);

    //create new env output
    createOutput(obstaclesOld);
    return true;
}

void StreetObstacleMerger::createOutput(std::vector<street_environment::Obstacle*> &obstaclesOld){
    envOutput->objects.clear();

    for(uint i = 0; i < obstaclesOld.size(); i++){
        //create a copy
        std::shared_ptr<street_environment::Obstacle> obstacle(new street_environment::Obstacle(*obstaclesOld[i]));
        envOutput->objects.push_back(obstacle);
    }

}

void StreetObstacleMerger::filter(std::vector<street_environment::Obstacle*> &obstaclesOld){
    //TODO hier könnte man sich auch etwas besseres einfallen lassen
    for(uint i = 0; i < obstaclesOld.size(); i++){
        if(obstaclesOld[i]->position().x < -0.35){
            obstaclesOld.erase(obstaclesOld.begin() + i);
        }
    }
}

void StreetObstacleMerger::merge(std::vector<street_environment::Obstacle*> &obstaclesNew,std::vector<street_environment::Obstacle*> &obstaclesOld){
    for(uint i = 0; i < obstaclesOld.size();){
        for(uint i = 0; i < obstaclesNew.size();){
            bool merged = merge(*obstaclesOld[i],*obstaclesNew[i]);
            if(merged){
                //remove merged obstacle
                obstaclesNew.erase(obstaclesNew.begin() + i);
                //no need to increase the index
            }else{
                //seems to be a new obstacle -> add it
                obstaclesOld.push_back(obstaclesNew[i]);
                //increase index
                i++;
            }
        }
    }
}

void StreetObstacleMerger::getObstacles(const street_environment::Environment &env,std::vector<street_environment::Obstacle*> &output){
    for(std::shared_ptr<street_environment::EnvironmentObject> obj : env.objects){
        if(obj->name().find("OBSTACLE") != std::string::npos){
            output.push_back(static_cast<street_environment::Obstacle*>(obj.get()));
        }
    }
}

bool StreetObstacleMerger::merge(street_environment::Obstacle &from, street_environment::Obstacle &to){
    //TODO, was besseres überlegen!
    if(from.position().distance(-to.position())<0.5){
        return true;
    }
    return false;
}

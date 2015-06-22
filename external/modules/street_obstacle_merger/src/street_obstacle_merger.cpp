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
    std::vector<street_environment::Obstacle*> obstaclesNew;
    std::vector<street_environment::Obstacle*> obstaclesOld;

    getObstacles(*envInput,obstaclesNew);
    getObstacles(*envOutput,obstaclesOld);
    for(uint i = 0; i < obstaclesOld.size(); i++){
        for(uint i = 0; i < obstaclesNew.size(); i++){
            bool merged = merge(*obstaclesOld[i],*obstaclesNew[i]);
            if(merged){
                //TODO do sth smart
            }else{
                //do nothing
            }
        }
    }

    //std::vector<street_environment::Obstacle&> cossings;
    //get all new found obstacles

    return true;
}

void StreetObstacleMerger::getObstacles(const street_environment::Environment &env,std::vector<street_environment::Obstacle*> &output){
    for(std::shared_ptr<street_environment::EnvironmentObject> obj : env.objects){
        if(obj->name().find("OBSTACLE") != std::string::npos){
            output.push_back(static_cast<street_environment::Obstacle*>(obj.get()));
        }
    }
}

bool StreetObstacleMerger::merge(street_environment::Obstacle &from, street_environment::Obstacle &to){
    //TODO
    return false;
}

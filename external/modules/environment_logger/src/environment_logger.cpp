#include "environment_logger.h"
#include <cmath>
#include <algorithm>
#include <fstream>
#include "lms/math/vertex.h"
bool EnvironmentLogger::initialize() {
    count = 0;
    input = readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_INPUT");
    directory = config().get<std::string>("directory");
    //TODO use lms logging!
    return true;
}


bool EnvironmentLogger::deinitialize() {
    return true;
}

bool EnvironmentLogger::cycle() {
    count++;
    for(std::shared_ptr<street_environment::EnvironmentObject> obj : input->objects){
        if(obj->getType() != street_environment::RoadLane::TYPE){
            continue;
        }
        const street_environment::RoadLane &lane = obj->getAsReference<const street_environment::RoadLane>();
        std::ofstream stream;
        stream.open(directory+"/"+"lines_"+std::to_string(count)+"_"+std::to_string((int)lane.type())+".csv");
        for(lms::math::vertex2f v : lane.points()){
            stream <<v.x<<","<<v.y<<"\n";
        }
        stream.close();
    }
    return true;
}

#include "environment_logger.h"
#include <cmath>
#include <algorithm>
#include "lms/datamanager.h"
#include <fstream>
#include "lms/math/vertex.h"
bool EnvironmentLogger::initialize() {
    count = 0;
    input = datamanager()->readChannel<Environment>(this, "ENVIRONMENT_INPUT");
    directory = getConfig()->get<std::string>("directory");
    return true;
}


bool EnvironmentLogger::deinitialize() {
    return true;
}

bool EnvironmentLogger::cycle() {
    count++;
    for(Environment::RoadLane lane : input->lanes){
        std::ofstream stream;
        stream.open(directory+"/"+"lines_"+std::to_string(count)+"_"+std::to_string((int)lane.type())+".csv");
        for(lms::math::vertex2f v : lane.points()){
            stream <<v.x<<","<<v.y<<"\n";
        }
        stream.close();
    }
    return true;
}

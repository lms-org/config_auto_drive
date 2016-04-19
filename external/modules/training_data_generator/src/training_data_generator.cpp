#include "training_data_generator.h"
bool TrainingDataGenerator::initialize() {
    road = readChannel<street_environment::RoadLane>("ROAD_OUTPUT");

    return true;
}

bool TrainingDataGenerator::deinitialize() {
    return true;
}

bool TrainingDataGenerator::cycle() {


    std::ofstream roadStream;
    roadStream.open(saveLogFile("roadStream_"+cycleCounter()));

    for(lms::math::vertex2f p : road->points()){
        roadStream<<p.x<<","<<p.y<<"\n";
    }
    roadStream.close();
    return true;
}

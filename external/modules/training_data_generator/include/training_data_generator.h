#ifndef TRAINING_DATA_GENERATOR_H
#define TRAINING_DATA_GENERATOR_H

#include <lms/module.h>
#include "street_environment/road.h"
#include <fstream>

/**
 * @brief LMS module training_data_generator
 **/
class TrainingDataGenerator : public lms::Module {

    lms::ReadDataChannel<street_environment::RoadLane> road;

public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // TRAINING_DATA_GENERATOR_H

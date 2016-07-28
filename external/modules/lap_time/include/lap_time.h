#ifndef LAP_TIME_H
#define LAP_TIME_H

#include <lms/module.h>
#include "street_environment/start_line.h"
#include "street_environment/car.h"

/**
 * @brief LMS module lap_time
 **/
class LapTime : public lms::Module {
    lms::ReadDataChannel<street_environment::EnvironmentObjects> environment;
    lms::ReadDataChannel<street_environment::Car> car;
    bool crossingStartLine;
    lms::Time start;
    float distanceDriven;
    float maxSpeed;
    float minSpeed;
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
    void crossedStartLine();
};

#endif // LAP_TIME_H

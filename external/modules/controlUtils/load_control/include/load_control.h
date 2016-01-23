#ifndef LOAD_CONTROL_H
#define LOAD_CONTROL_H

#include <lms/module.h>
#include "sensor_utils/car.h"
/**
 * @brief LMS module load_control
 **/
class LoadControl : public lms::Module {
    lms::WriteDataChannel<sensor_utils::Car::State> carState;

public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // LOAD_CONTROL_H

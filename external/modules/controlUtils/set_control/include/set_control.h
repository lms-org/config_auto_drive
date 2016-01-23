#ifndef SET_CONTROL_H
#define SET_CONTROL_H

#include <lms/module.h>
#include "sensor_utils/car.h"
/**
 * @brief LMS module set_control
 **/
class SetControl : public lms::Module {
 lms::WriteDataChannel<sensor_utils::Car> car;
 lms::ReadDataChannel<sensor_utils::Car::State> carState;
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // SET_CONTROL_H

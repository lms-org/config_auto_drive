#ifndef LOAD_CONTROL_H
#define LOAD_CONTROL_H

#include <lms/module.h>
#include "street_environment/car.h"
/**
 * @brief LMS module load_control
 **/
class LoadControl : public lms::Module {
    lms::WriteDataChannel<street_environment::CarCommand::State> carState;

public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // LOAD_CONTROL_H

#ifndef SET_CONTROL_H
#define SET_CONTROL_H

#include <lms/module.h>
#include "street_environment/car.h"
/**
 * @brief LMS module set_control
 **/
class SetControl : public lms::Module {
 lms::WriteDataChannel<street_environment::CarCommand> car;
 lms::ReadDataChannel<street_environment::CarCommand::State> carState;
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // SET_CONTROL_H

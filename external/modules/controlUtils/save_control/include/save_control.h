#ifndef SAVE_CONTROL_H
#define SAVE_CONTROL_H

#include <lms/module.h>
#include "street_environment/car.h"

/**
 * @brief LMS module save_control
 **/
class SaveControl : public lms::Module {
    lms::ReadDataChannel<street_environment::Car::State> debugRcCarState;
    bool isRecording;
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
    bool startRecord();
    bool stopRecord();
};

#endif // SAVE_CONTROL_H

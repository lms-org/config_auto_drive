#ifndef SAVE_CONTROL_H
#define SAVE_CONTROL_H

#include <lms/module.h>

/**
 * @brief LMS module save_control
 **/
class SaveControl : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // SAVE_CONTROL_H

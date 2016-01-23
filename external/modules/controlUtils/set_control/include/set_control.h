#ifndef SET_CONTROL_H
#define SET_CONTROL_H

#include <lms/module.h>

/**
 * @brief LMS module set_control
 **/
class SetControl : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // SET_CONTROL_H

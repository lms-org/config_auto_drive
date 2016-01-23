#ifndef LOAD_CONTROL_H
#define LOAD_CONTROL_H

#include <lms/module.h>

/**
 * @brief LMS module load_control
 **/
class LoadControl : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // LOAD_CONTROL_H

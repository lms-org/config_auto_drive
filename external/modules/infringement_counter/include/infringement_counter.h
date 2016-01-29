#ifndef INFRINGEMENT_COUNTER_H
#define INFRINGEMENT_COUNTER_H

#include <lms/module.h>

/**
 * @brief LMS module infringement_counter
 **/
class InfringementCounter : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // INFRINGEMENT_COUNTER_H

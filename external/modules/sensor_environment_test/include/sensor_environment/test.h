#ifndef ENV_TEST_H
#define ENV_TEST_H

#include <lms/module.h>
#include <sensor_environment/sensor_environment.h>
class EnvTest:public lms::Module{
    LinedDepthEnvironment *lde;

public:
    bool initialize();
    bool deinitialize();
    bool cycle();
};

#endif /* ENV_TEST_H */

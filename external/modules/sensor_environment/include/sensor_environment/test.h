#ifndef ENV_TEST_H
#define ENV_TEST_H
#include <lms/module.h>
class EnvTest:public lms::Module{
public:
    bool initialize();
    bool deinitialize();

    bool cycle();
};

#endif /* ENV_TEST_H */

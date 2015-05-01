#include <sensor_environment/test.h>

extern "C" {
void* getInstance () {
    return new EnvTest();
}
}

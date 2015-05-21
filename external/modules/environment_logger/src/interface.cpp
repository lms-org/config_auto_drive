#include "environment_logger.h"

extern "C" {

void* getInstance() {
    return new EnvironmentLogger();
}
}

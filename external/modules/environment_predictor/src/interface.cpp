#include <environment_predictor.h>

extern "C" {
void* getInstance () {
    return new EnvironmentPredictor();
}
}

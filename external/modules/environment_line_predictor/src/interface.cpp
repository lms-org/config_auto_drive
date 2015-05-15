#include "environment_line_predictor.h"

extern "C" {
void* getInstance() {
    return new EnvironmentLinePredictor();
}
}

#include "car_to_matlab.h"

extern "C" {

void* getInstance() {
    return new CarToMatlab();
}
}

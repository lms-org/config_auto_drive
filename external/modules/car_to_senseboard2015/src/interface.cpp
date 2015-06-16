#include "car_to_senseboard2015.h"

extern "C" {

void* getInstance() {
    return new CarToSenseboard2015();
}
}

#include "car_tracker.h"

extern "C" {

void* getInstance() {
    return new CarTracker();
}
}

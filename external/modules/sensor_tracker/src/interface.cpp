#include "sensor_tracker.h"

extern "C" {

void* getInstance() {
    return new SensorTracker();
}

}

#include "street_obstacle_ir_detector.h"

extern "C" {

void* getInstance() {
    return new StreetObstacleIRDetector();
}

}

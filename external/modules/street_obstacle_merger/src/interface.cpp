#include "street_obstacle_merger.h"

extern "C" {

void* getInstance() {
    return new StreetObjectMerger();
}

}

#include "trajectory_point_creator.h"

extern "C" {

void* getInstance() {
    return new TrajectoryPointCreator();
}

}

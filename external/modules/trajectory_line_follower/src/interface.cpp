#include "line_trajectory_follower.h"

extern "C" {

void* getInstance() {
    return new TrajectoryLineFollower();
}

}

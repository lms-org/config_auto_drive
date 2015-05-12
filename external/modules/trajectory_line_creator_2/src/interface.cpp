#include "line_trajectory_creator.h"

extern "C" {

void* getInstance() {
    return new TrajectoryLineCreator();
}

}

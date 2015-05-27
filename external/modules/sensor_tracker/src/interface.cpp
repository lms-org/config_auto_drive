#include "trajectory_line_creator.h"

extern "C" {

void* getInstance() {
    return new TrajectoryLineCreator();
}

}

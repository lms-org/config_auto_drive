#include "load_control.h"

bool LoadControl::initialize() {
    carState = writeChannel<street_environment::CarCommand::State>("CAR_STATE");
    return true;
}

bool LoadControl::deinitialize() {
    return true;
}

bool LoadControl::cycle() {
    //TODO set the data for carState
    return true;
}

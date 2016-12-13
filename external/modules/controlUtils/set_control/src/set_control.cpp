#include "set_control.h"

bool SetControl::initialize() {
    carState = readChannel<street_environment::CarCommand::State>("CAR_STATE");
    car = writeChannel<street_environment::CarCommand>("CAR");
    return true;
}

bool SetControl::deinitialize() {
    return true;
}

bool SetControl::cycle() {
    //awesome cycle method
    car->putState(*carState);
    return true;
}

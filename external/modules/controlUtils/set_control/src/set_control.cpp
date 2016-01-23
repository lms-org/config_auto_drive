#include "set_control.h"

bool SetControl::initialize() {
    carState = readChannel<sensor_utils::Car::State>("CAR_STATE");
    car = writeChannel<sensor_utils::Car>("CAR");
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

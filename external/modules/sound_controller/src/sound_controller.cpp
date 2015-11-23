#include "sound_controller.h"

bool SoundController::initialize() {
    motorsounds = config().getArray<std::string>("sounds");
    speed = config().getArray<float>("speed");
    return true;
}

bool SoundController::deinitialize() {
    return true;
}

bool SoundController::cycle() {
    //TODO
    return true;
}

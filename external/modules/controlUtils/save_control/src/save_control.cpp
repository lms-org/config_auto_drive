#include "save_control.h"

bool SaveControl::initialize() {
    debugRcCarState = readChannel<street_environment::Car::State>("RC_CAR_STATE");
    isRecording = false;
    return true;
}

bool SaveControl::deinitialize() {
    return true;
}

bool SaveControl::cycle() {
    //TODO check msgs to startRecord and stopRecord

    //TODO we could use https://github.com/lms-org/channel_serializer

    //TODO save debugRcCarState with cycle/timestamp/whatever to replay it

    return true;
}



bool SaveControl::startRecord(){
    isRecording = true;
    //create new file to store data
    return true;
}

bool SaveControl::stopRecord(){
    isRecording = false;
    return true;
}

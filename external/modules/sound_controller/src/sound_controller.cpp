#include "sound_controller.h"

bool SoundController::initialize() {
    motorsounds = config().getArray<std::string>("sounds");
    speed = config().getArray<float>("speed");
    playList = writeChannel<soundUtils::Sounds>("PLAYLIST");
    return true;
}

bool SoundController::deinitialize() {
    return true;
}

bool SoundController::cycle() {
   float currentSpeed = 1;
   playList->clear();
   for(int i = 0; i< speed.size();i++){
        float factor =  (  currentSpeed / speed[i]);
        logger.info("cycle") << factor;
        soundUtils::SoundFile sf;
        sf.loop = true;
        sf.name = motorsounds[i];
        sf.pitch = factor;
        sf.volume = 100; //TODO set volume
        if(sf.volume > 0){
            playList->push_back(sf);
        }
    }
    return true;
}

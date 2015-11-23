#include "sound_controller.h"

extern "C" {
void* getInstance () {
    return new SoundController();
}
}

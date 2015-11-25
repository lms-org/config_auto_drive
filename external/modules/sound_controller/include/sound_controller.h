#ifndef SOUND_CONTROLLER_H
#define SOUND_CONTROLLER_H

#include <lms/datamanager.h>
#include <lms/module.h>

/**
 * @brief LMS module sound_controller
 **/
class SoundController : public lms::Module {
private:
    std::vector<std::string> motorsounds;
    std::vector<float> speed;
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // SOUND_CONTROLLER_H

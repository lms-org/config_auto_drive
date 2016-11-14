#ifndef CAR_TRACK_VISULIZER_H
#define CAR_TRACK_VISULIZER_H

#include <lms/module.h>

/**
 * @brief LMS module car_track_visulizer
 **/
class CarTrackVisulizer : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // CAR_TRACK_VISULIZER_H

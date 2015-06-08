#ifndef LMS_TRAJECTORY_POINT_CREATOR_H
#define LMS_TRAJECTORY_POINT_CREATOR_H

#include "lms/module.h"
#include "lms/math/polyline.h"
#include "lms/math/math.h"
#include "image_objects/environment.h"
#include "senseboard2015.h"

class VelocityController : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    const Environment *envInput;
    const lms::type::ModuleConfig *config;
    Comm::SensorBoard::ControlData *controlData;
    lms::extra::PrecisionTime lastCall;
    /**
     * @brief launchControll
     * @param newVeolocity
     * @param currentVelocity
     * @return true if launchControll is active, false if not
     */
    bool launchControll(float newVeolocity,float currentVelocity);
    bool defaultDrive();

};

#endif /* LMS_TRAJECTORY_POINT_CREATOR_H */

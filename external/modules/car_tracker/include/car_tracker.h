#ifndef CAR_TRACKER_H
#define CAR_TRACKER_H

#include "lms/module.h"
#include "comm/senseboard.h"
#include "lms/extra/time.h"

class CarTracker : public lms::Module {
    struct DeltaState{
        DeltaState():x(0),y(0),phi(0),valid(false){
        }
        bool valid;
        float x;
        float y;
        float phi;
    };

public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;

private:
    Comm::SensorBoard::ControlData *controlData;

    void getFromVehicle(DeltaState &d);
    void getFromTrajectory(DeltaState &d);
    void getFromMouseSensors(DeltaState &d);
    float delta;
    lms::extra::PrecisionTime last;
    bool firstRun;
};

#endif /* CAR_TRACKER */

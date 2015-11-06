#ifndef CAR_TRACKER_H
#define CAR_TRACKER_H

#include "lms/module.h"
#include "comm/senseboard.h"
#include "lms/extra/time.h"
#include "sensor_utils/car.h"
/**
 * @brief The CarTracker class
 * tracks the motion of our rc car
 */
class CarTracker : public lms::Module {
    struct DeltaState{
        DeltaState():valid(false),x(0),y(0),phi(0){
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

    void getFromVehicle(DeltaState &d);
    void getFromTrajectory(DeltaState &d);
    void getFromMouseSensors(DeltaState &d);
    void getFromImu(DeltaState &d);
    float delta;
    lms::extra::PrecisionTime last;
    bool firstRun;
    lms::WriteDataChannel<sensor_utils::Car> car;
};

#endif /* CAR_TRACKER */

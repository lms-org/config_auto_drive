#ifndef CAR_TO_MATLAB_H
#define CAR_TO_MATLAB_H

#include "lms/module.h"
#include "comm/senseboard.h"
#include "lms/extra/time.h"
#include "sensor_utils/car.h"

class CarToMatlab : public lms::Module {
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
    const sensor_utils::Car *car;
};

#endif /* CAR_TO_MATLAB_H */

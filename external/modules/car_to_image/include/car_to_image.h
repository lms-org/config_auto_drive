#ifndef CAR_TO_IMAGE_H
#define CAR_TO_IMAGE_H

#include "lms/module.h"
#include "lms/extra/time.h"
#include "sensor_utils/car.h"
#include "lms/imaging/image.h"

class CarToImage : public lms::Module {
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
    lms::ReadDataChannel<sensor_utils::Car> car;
    lms::WriteDataChannel<lms::imaging::Image> image;
    float meterToPxl;
    int offsetX;
    int offsetY;
};

#endif /* CAR_TO_MATLAB_H */

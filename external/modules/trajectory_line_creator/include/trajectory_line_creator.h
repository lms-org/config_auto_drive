#ifndef IMAGE_HINT_TRANSFORMER_H
#define IMAGE_HINT_TRANSFORMER_H

#include "lms/module.h"
#include "lms/math/polyline.h"
#include "street_environment/road.h"
#include "sensor_utils/car.h"

class TrajectoryLineCreator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    const street_environment::Environment *environmentMiddle;
    const street_environment::Environment *environment;
    lms::math::polyLine2f *line;
    const lms::type::ModuleConfig *config;
    sensor_utils::Car *car;

};

#endif /* IMAGE_HINT_TRANSFORMER_H */

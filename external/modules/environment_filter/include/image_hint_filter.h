#ifndef IMAGE_HINT_FILTER_H
#define IMAGE_HINT_FILTER_H

#include "lms/module.h"
#include "street_environment/road.h"
#include "street_environment/street_environment.h"

class EnvironmentFilter : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    void filterLane(street_environment::RoadLane &lane);
    const street_environment::EnvironmentObjects *input;
    street_environment::EnvironmentObjects *output;
};

#endif /* IMAGE_HINT_FILTER_H */

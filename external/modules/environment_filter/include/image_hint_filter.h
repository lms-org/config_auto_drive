#ifndef IMAGE_HINT_FILTER_H
#define IMAGE_HINT_FILTER_H

#include "lms/module.h"
#include "image_objects/environment.h"

class EnvironmentFilter : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    void filterLane(Environment::RoadLane &lane, bool checkAngle, bool checkDistance);
    const Environment *input;
    Environment *output;
};

#endif /* IMAGE_HINT_FILTER_H */

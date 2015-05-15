#ifndef ENVIRONMENT_LINE_PREDICTOR_H
#define ENVIRONMENT_LINE_PREDICTOR_H

#include "lms/module.h"
#include "lms/datamanager.h"
#include "image_objects/environment.h"

class EnvironmentLinePredictor : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    void predict(const Environment::RoadLane &input, Environment::RoadLane &output);

    const Environment *input;
    Environment *output;
};

#endif /* ENVIRONMENT_LINE_PREDICTOR_H */

#ifndef ENVIRONMENT_PREDICTOR_H
#define ENVIRONMENT_PREDICTOR_H

#include "lms/module.h"
#include "lms/imaging/image.h"
#include "lms/imaging/format.h"

class EnvironmentPredictor : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:

};

#endif /* IMAGE_CONVERTER_H */

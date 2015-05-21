#ifndef ENVIRONMENT_LOGGER_H
#define ENVIRONMENT_LOGGER_H

#include "lms/module.h"
#include "image_objects/environment.h"

class EnvironmentLogger : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    const Environment *input;
    int count;
    std::string directory;
};

#endif /* ENVIRONMENT_LOGGER_H */

#ifndef ENVIRONMENT_LOGGER_H
#define ENVIRONMENT_LOGGER_H

#include "lms/module.h"
#include "street_environment/road.h"

class EnvironmentLogger : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    lms::ReadDataChannel<street_environment::EnvironmentObjects> input;
    int count;
    std::string directory;
};

#endif /* ENVIRONMENT_LOGGER_H */

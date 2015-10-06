#ifndef ENVIRONMENT_PREDICTOR_H
#define ENVIRONMENT_PREDICTOR_H

#include "lms/module.h"
#include "lms/imaging/image.h"
#include "lms/imaging/format.h"
#include "street_environment/road.h"
#include "lms/type/module_config.h"
#include "sensor_utils/car.h"
#include <fstream>

#include "kalman_filter_lr_emxAPI.h"


class EnvironmentPredictor : public lms::Module {

public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    void resetData();
    const street_environment::EnvironmentObjects *envInput;
    street_environment::RoadLane *roadOutput;
    const sensor_utils::Car *car;

    emxArray_real_T *zustandsVector;
    int partCount;
    double partLength;
    const lms::type::ModuleConfig *config;
    void convertToKalmanArray(const street_environment::RoadLane &lane,emxArray_real_T **x,emxArray_real_T **y);
    void asEinheitsMatrix(emxArray_real_T *mat);
    void clearMatrix(emxArray_real_T *mat);
    emxArray_real_T *stateTransitionMatrix;

    emxArray_real_T *kovarianzMatrixDesZustandes;
    emxArray_real_T *kovarianzMatrixDesZustandUebergangs;
    double r_fakt; //messgenauigkeit
    void createOutput();
    void convertZustandToLane(street_environment::RoadLane &output);

    void printMat(emxArray_real_T *mat);
    void logStateVector();
    
    std::ofstream logFile;
    size_t cycleCounter;
};

#endif /* IMAGE_CONVERTER_H */

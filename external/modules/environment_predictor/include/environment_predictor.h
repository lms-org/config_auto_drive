#ifndef ENVIRONMENT_PREDICTOR_H
#define ENVIRONMENT_PREDICTOR_H

#include "lms/module.h"
#include "lms/imaging/image.h"
#include "lms/imaging/format.h"
#include "image_objects/environment.h"
extern "C"{
#include "kalman_filter_lr_emxAPI.h"
}

class EnvironmentPredictor : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    const Environment *envInput;
    Environment *envOutput;
    void convertToKalmanArray(const Environment::RoadLane &lane,emxArray_real_T **x,emxArray_real_T **y);
    void asEinheitsMatrix(emxArray_real_T *mat);
    void clearMatrix(emxArray_real_T *mat);


    int n;
    double delta;
    emxArray_real_T *zustandsVector;
    double r[10];
    emxArray_real_T *stateTransitionMatrix;

    emxArray_real_T *kovarianzMatrixDesZustandes;
    emxArray_real_T *kovarianzMatrixDesZustandUebergangs;
    double r_fakt; //messgenauigkeit
    void createOutput();
    void convertZustandToLane(Environment::RoadLane &output);

    void printMat(emxArray_real_T *mat);
};

#endif /* IMAGE_CONVERTER_H */

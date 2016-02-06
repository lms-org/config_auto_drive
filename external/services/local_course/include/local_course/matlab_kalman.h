#ifndef LOCAL_COURSE_SERVICE_MATLAB_KALMAN
#define LOCAL_COURSE_SERVICE_MATLAB_KALMAN

#include "lms/math/vertex.h"
#include <vector>
#include "lms/logger.h"
#include "lms/config.h"
#include "lms/math/polyline.h"
#include "street_environment/road.h"
struct emxArray_real_T;

namespace local_course {
class MatlabKalman{
    lms::logging::Logger &logger;
    emxArray_real_T *zustandsVector;
    int partCount;
    double partLength;
    float prior_fact;
    void convertToKalmanArray(const std::vector<lms::math::vertex2f> &lane,emxArray_real_T **x,emxArray_real_T **y);
    void asEinheitsMatrix(emxArray_real_T *mat, double val);
    void clearMatrix(emxArray_real_T *mat);
    emxArray_real_T *stateTransitionMatrix;

    emxArray_real_T *kovarianzMatrixDesZustandes;
    emxArray_real_T *kovarianzMatrixDesZustandUebergangs;
    double r_fakt; //messgenauigkeit
    void createOutput(street_environment::RoadLane &line);

    void printMat(emxArray_real_T *mat);

    int resetCounter;

public:
    MatlabKalman(lms::logging::Logger &logger);
    void configsChanged(const lms::Config &config);
    void resetData(const lms::Config &config);
    void logStateVector(std::ostream &logFile);
    bool update(std::vector<lms::math::vertex2f> points, float dx, float dy, float dphi, float measurementUncertainty);
    street_environment::RoadLane getOutput();


};
}//namespace local_course


#endif //LOCAL_COURSE_SERVICE_MATLAB_KALMAN

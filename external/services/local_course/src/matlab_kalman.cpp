#include "local_course/matlab_kalman.h"
#include <string>
#include "lms/imaging/converter.h"
#include <lms/imaging/image_factory.h>
#include "street_environment/road.h"
#include "street_environment/crossing.h"
#include "cmath"

#include "kalman_filter_lr_emxAPI.h"
#include "kalman_filter_lr.h"

namespace local_course{
MatlabKalman::MatlabKalman(lms::logging::Logger &logger_):logger(logger_) {
    zustandsVector = nullptr;
    stateTransitionMatrix = nullptr;
    kovarianzMatrixDesZustandes = nullptr;
    kovarianzMatrixDesZustandUebergangs = nullptr;
    resetCounter = 0;
}


void MatlabKalman::configsChanged(const lms::Config &config){
    prior_fact = config.get<float>("prior_fact",0);
    partCount = config.get<int>("elementCount",10);
    partLength = config.get<float>("elementLength",0.2);

    if(zustandsVector != nullptr){
        emxDestroyArray_real_T(zustandsVector);
    }
    if(stateTransitionMatrix != nullptr){
        emxDestroyArray_real_T(stateTransitionMatrix);
    }
    if(kovarianzMatrixDesZustandes != nullptr){
        emxDestroyArray_real_T(kovarianzMatrixDesZustandes);
    }
    if(kovarianzMatrixDesZustandUebergangs != nullptr){
        emxDestroyArray_real_T(kovarianzMatrixDesZustandUebergangs);
    }
    zustandsVector = emxCreate_real_T(partCount,1);
    stateTransitionMatrix = emxCreate_real_T(partCount,partCount);
    kovarianzMatrixDesZustandes = emxCreate_real_T(partCount,partCount);
    kovarianzMatrixDesZustandUebergangs = emxCreate_real_T(partCount,partCount);
    r_fakt=config.get<double>("r_fakt",20);
    resetData(config);
}



void MatlabKalman::resetData(const lms::Config &config){
    logger.info("resetData");
    clearMatrix(zustandsVector);
    zustandsVector->data[0] = config.get<float>("distanceToMiddle",0.2);
    asEinheitsMatrix(stateTransitionMatrix, 1);
    asEinheitsMatrix(kovarianzMatrixDesZustandes, 1);
    asEinheitsMatrix(kovarianzMatrixDesZustandUebergangs, config.get<float>("kov",15));
    //clearMatrix(kovarianzMatrixDesZustandUebergangs);

    resetCounter = 0;

    for(int x = 0; x < partCount; x++){
        for(int y = 0; y < partCount; y++){
            if (x != 0 && y != 0) kovarianzMatrixDesZustandUebergangs->data[y*partCount+x]=config.get<float>("kov",15)*(1-pow(config.get<float>("kovAbnahme",0.2),1/fabs(x-y)));
        }
    }
}

bool MatlabKalman::update(std::vector<lms::math::vertex2f> points, float dx, float dy, float dphi, float measurementUncertainty) {

    if (++resetCounter < 10) measurementUncertainty = 10.0;

    //länge der später zu berechnenden Abschnitten
    //convert data to lines
    emxArray_real_T *rx = nullptr;
    emxArray_real_T *ry = nullptr;
    emxArray_real_T *lx = nullptr;
    emxArray_real_T *ly = nullptr;
    emxArray_real_T *mx = nullptr;
    emxArray_real_T *my = nullptr;



    //convert points
    convertToKalmanArray(points,&mx,&my);
    if(rx == nullptr){
        logger.debug("right has zero points");
        rx = emxCreate_real_T(0,0);
        ry = emxCreate_real_T(0,0);
    }
    if(lx == nullptr){
        logger.debug("left has zero points");
        lx = emxCreate_real_T(0,0);
        ly = emxCreate_real_T(0,0);
    }
    if(mx == nullptr){
        logger.debug("middle has zero points");
        mx = emxCreate_real_T(0,0);
        my = emxCreate_real_T(0,0);
    }

    /* TODO
    if(car->getPrioState().state == sensor_utils::Car::StateType::IDLE){
        //Auto steht an kreuzung, wir gehen davon aus, dass die Straße im Nahbereich gerade ist
        prior_fact = config().get<float>("idle_prior_fact",1);
    }
    */

    //TODO
    prior_fact = 0;

    //TODO don't limit changes
    float y0_before = zustandsVector->data[0];
    float phi0_before = zustandsVector->data[1];

    kalman_filter_lr(zustandsVector,dx,dy,dphi,kovarianzMatrixDesZustandes,
                     kovarianzMatrixDesZustandUebergangs,
                     measurementUncertainty,partLength,lx,ly,rx,ry,mx,my,1,prior_fact);

    //TODO don't limit changes
    if (fabs(zustandsVector->data[0] - y0_before) > 0.05 )     zustandsVector->data[0] = y0_before;
    if (fabs(zustandsVector->data[1] - phi0_before) > 0.1 )    zustandsVector->data[1] = phi0_before;

    //destroy stuff
    emxDestroyArray_real_T(rx);
    emxDestroyArray_real_T(ry);
    emxDestroyArray_real_T(lx);
    emxDestroyArray_real_T(ly);
    emxDestroyArray_real_T(mx);
    emxDestroyArray_real_T(my);

    return true;
}

void MatlabKalman::logStateVector(std::ostream &logFile){
    for(int i = 0; i < zustandsVector->size[0]; i++)
    {
        logFile << "," << zustandsVector->data[i];
    }

    logFile << std::endl;
}

street_environment::RoadLane MatlabKalman::getOutput(){
    //create middle
    logger.debug("createOutput");
    street_environment::RoadLane output;
    createOutput(output);
    return output;
}

void MatlabKalman::createOutput(street_environment::RoadLane &output){
    //clear points
    logger.debug("convertZustandToLane ANFANG");
    output.points().clear();
    logger.debug("convertZustandToLane ANFANG")<<1;
    output.polarDarstellung.clear();
    logger.debug("convertZustandToLane CLEARED OLD VALS");

    lms::math::vertex2f p1;
    p1.x = 0;
    p1.y = zustandsVector->data[0];
    lms::math::vertex2f p2;
    p2.x = partLength*cos(zustandsVector->data[1]);
    p2.y = p1.y + partLength*sin(zustandsVector->data[1]);
    double phi = zustandsVector->data[1];
    //add points to lane
    output.points().push_back(p1);
    output.points().push_back(p2);
    output.polarPartLength = partLength;
    //add first two values
    output.polarDarstellung.push_back(zustandsVector->data[0]);
    output.polarDarstellung.push_back(zustandsVector->data[1]);

    for(int i = 2; i < partCount; i++){
        lms::math::vertex2f pi;
        double dw = 2*acos(partLength*zustandsVector->data[i]/2);
        phi = phi -dw+M_PI;
        pi.x = output.points()[i-1].x + partLength*cos(phi);
        pi.y = output.points()[i-1].y + partLength*sin(phi);
        output.points().push_back(pi);
        logger.debug("points: ")<<"krümmung: "<<zustandsVector->data[i]<< " ,dw "<<dw<<" ,x:  " <<pi.x << " ,y: "<<pi.y;
        output.polarDarstellung.push_back(zustandsVector->data[i]);
    }

}

void MatlabKalman::clearMatrix(emxArray_real_T *mat){
    memset(mat->data,0,mat->size[0]*mat->size[1]*sizeof(double));

}

void MatlabKalman::asEinheitsMatrix(emxArray_real_T *mat, double val){
    clearMatrix(mat);
    for(int i = 0; i < mat->size[0]; i++){
        mat->data[i*(mat->size[0]+1)] = val;
    }
}

void MatlabKalman::convertToKalmanArray(const std::vector<lms::math::vertex2f> &lane,emxArray_real_T **x,emxArray_real_T **y){
    int dim = lane.size();
    emxArray_real_T *vx = emxCreate_real_T(dim,1);
    emxArray_real_T *vy = emxCreate_real_T(dim,1);
    for(uint i=0;i < lane.size(); i++){
        vx->data[i] = lane[i].x;
        vy->data[i] = lane[i].y;
    }
    *x = vx;
    *y = vy;
}

void MatlabKalman::printMat(emxArray_real_T *mat){
    std::cout<<"mat: "<<std::endl;
    for(int x = 0; x < mat->size[0];x++){
        for(int y = 0; y < mat->size[1]; y++){
            std::cout << mat->data[x*mat->size[1]+y];
            std::cout <<",";
        }
        std::cout <<std::endl;
    }
}
}//namespace local_course

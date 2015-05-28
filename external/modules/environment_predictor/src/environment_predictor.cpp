#include <string>

#include "environment_predictor.h"
#include "lms/datamanager.h"
#include "lms/imaging/converter.h"
#include <lms/imaging/image_factory.h>
#include "lms/imaging/warp.h"
#include "image_objects/environment.h"
#include "cmath"
extern "C"{
#include "kalman_filter_lr.h"
}
bool EnvironmentPredictor::initialize() {
    envInput = datamanager()->readChannel<Environment>(this,"ENVIRONMENT_INPUT");
    envOutput = datamanager()->writeChannel<Environment>(this,"ENVIRONMENT_OUTPUT");

    n = 10;
    delta = 0.2;
    zustandsVector = emxCreate_real_T(n,1);
    for(int i = 0; i < 10; i++){
        r[i]=0;
    }
    //logger.info("SIZES: ") << zustandsVector->size[0] <<" "<<zustandsVector->size[1];
    clearMatrix(zustandsVector);
    stateTransitionMatrix = emxCreate_real_T(n,n);
    asEinheitsMatrix(stateTransitionMatrix);
    kovarianzMatrixDesZustandes = emxCreate_real_T(n,n);
    asEinheitsMatrix(kovarianzMatrixDesZustandes);
    kovarianzMatrixDesZustandUebergangs = emxCreate_real_T(n,n);
    clearMatrix(kovarianzMatrixDesZustandUebergangs);

    for(int x = 0; x < n; x++){
        for(int y = 0; y < n; y++){
            kovarianzMatrixDesZustandUebergangs->data[y*n+x]=15*(1-pow(0.2,1/fabs(x-y)));
        }
    }
    printMat(zustandsVector);
    printMat(stateTransitionMatrix);
    printMat(kovarianzMatrixDesZustandes);
    printMat(kovarianzMatrixDesZustandUebergangs);
    r_fakt = 1; //messgenauigkeit
    return true;
}

bool EnvironmentPredictor::deinitialize() {
    return true;
}

bool EnvironmentPredictor::cycle() {
    //länge der später zu berechnenden Abschnitten
    //convert data to lines
    emxArray_real_T *rx;
    emxArray_real_T *ry;
    emxArray_real_T *lx;
    emxArray_real_T *ly;
    for(const Environment::RoadLane &rl :envInput->lanes){
        if(rl.type() == Environment::RoadLaneType::LEFT){
            logger.debug("cycle") << "found left lane: " << rl.points().size();
            convertToKalmanArray(rl,&lx,&ly);
        }else if(rl.type() == Environment::RoadLaneType::RIGHT){
            logger.debug("cycle") << "found right lane: " << rl.points().size();
            convertToKalmanArray(rl,&rx,&ry);
        }
    }
    //kalman everything
    //printMat(zustandsVector);

    kalman_filter_lr(r,stateTransitionMatrix,kovarianzMatrixDesZustandes,
                     kovarianzMatrixDesZustandUebergangs,
                     r_fakt,delta,lx,ly,rx,ry);

    std::cout <<"Zustand danach: ";
    //printMat(zustandsVector);
    createOutput();
    //destroy stuff
    emxDestroyArray_real_T(rx);
    emxDestroyArray_real_T(ry);
    emxDestroyArray_real_T(lx);
    emxDestroyArray_real_T(ly);
    return true;
}

void EnvironmentPredictor::createOutput(){
    Environment::RoadLane middle;
    lms::math::vertex2f p1;
    p1.x = 0;
    p1.y = r[0];//zustandsVector->data[0];
    lms::math::vertex2f p2;
    p2.x = delta*cos(r[1]);//zustandsVector->data[1]);
    p2.y = p1.y + delta*r[1];//sin(zustandsVector->data[1]);
    double phi = r[1];//zustandsVector->data[2];
    middle.points().push_back(p1);
    middle.points().push_back(p2);
    for(int i = 2; i < n; i++){
        lms::math::vertex2f pi;
        double dw = 2*acos(delta*r[i]/*zustandsVector->data[i]*//2);
        phi = phi -dw-M_PI;
        pi.x = middle.points()[i-1].x + delta*cos(phi);
        pi.y = middle.points()[i-1].y + delta*sin(phi);
        middle.points().push_back(pi);
    }
    envOutput->lanes.clear();
    envOutput->lanes.push_back(middle);
}

void EnvironmentPredictor::clearMatrix(emxArray_real_T *mat){
    memset(mat->data,0,mat->size[0]*mat->size[1]*sizeof(double));

}

void EnvironmentPredictor::asEinheitsMatrix(emxArray_real_T *mat){
    clearMatrix(mat);
    for(int i = 0; i < mat->size[0]; i++){
        mat->data[i*(mat->size[0]+1)] = 1;
    }
}

void EnvironmentPredictor::convertToKalmanArray(const Environment::RoadLane &lane,emxArray_real_T **x,emxArray_real_T **y){
    int dim = lane.points().size();
    emxArray_real_T *vx = emxCreate_real_T(dim,1);
    emxArray_real_T *vy = emxCreate_real_T(dim,1);
    for(uint i=0;i < lane.points().size(); i++){
        vx->data[i] = lane.points()[i].x;
        vy->data[i] = lane.points()[i].y;
    }
    *x = vx;
    *y = vy;
}

void EnvironmentPredictor::printMat(emxArray_real_T *mat){
    std::cout<<"mat: "<<std::endl;
    for(int x = 0; x < mat->size[0];x++){
        for(int y = 0; y < mat->size[1]; y++){
            std::cout << mat->data[x*mat->size[1]+y];
            std::cout <<",";
        }
        std::cout <<std::endl;
    }
}

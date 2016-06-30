#include "local_course/ekf.h"



void ExtendedKalmanFilterForLine::computeMeasurement(){
    //TODO
}

void ExtendedKalmanFilterForLine::computeFilterStep(){
    //TODO
}

void ExtendedKalmanFilterForLine::initFilter(){
    //TODO
}

void ExtendedKalmanFilterForLine::init(const Eigen::VectorXf &points, const float partLength){
    this->partLength = partLength;
    //TODO
}

void ExtendedKalmanFilterForLine::update(const Eigen::VectorXf &points,const float dx,const float dy,const float dphi){
    computeMeasurement();
    computeFilterStep();
    //Fuer jeden Messpunkt den naechsten Punkt der aktuellen Praediktion finden

    //Punkte welche zu weit weg liegen entfernen
    //Wir wollen x0 auf 0 haben
    //y0 variabel
    //danach winkel zum vorherigen Stück

}

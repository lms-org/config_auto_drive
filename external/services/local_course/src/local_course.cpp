#include "local_course/local_course.h"

#include <filter/line_line_x.h>
#include <unistd.h>
#include <filter/adam.h>
#include <cmath>

namespace local_course {

LocalCourse::LocalCourse(){

}

bool LocalCourse::init() {
    lineX = new LineX();
    configsChanged();
    return true;
}

void LocalCourse::destroy() {
    //TODO
}

void LocalCourse::configsChanged(){
    lineX->init(config().get<int>("elementCount",10));
    lineX->state(1)=config().get<float>("yOffset",0.2); //set y-offset
    lineX->lineLength = config().get<float>("elementLength",0.2);
    lineX->fixX = config().get<float>("fixX",true);
    lineX->fixY = config().get<float>("fixY",false);

    outlierStartingState = config().get<int>("outlierStartingPoint", 1);
    outlierPercentile = config().get<float>("outlierPercentile", 0.5);
    outlierPercentileMultiplier = config().get<float>("outlierPercentileMultiplier", 3.0);
}

void LocalCourse::update(float dx, float dy, float dphi){
    //remove outliers TODO move to other module/function!
    /*
    for(int i = 0; i < lineX->state.rows()*lineX->state.cols(); i++){
        if(std::isnan(lineX->state(i))){
            logger.error("update")<<"BEGIN: state is nan, index: "<<i;
            exit(0);
        }
    }
    */
    lineX->translate(dx,dy,dphi);
    /*
    for(int i = 0; i < lineX->state.rows()*lineX->state.cols(); i++){
        if(std::isnan(lineX->state(i))){
            logger.error("update")<<"AFTER TRANSLATION: state is nan, index: "<<i;
            exit(0);
        }
    }
    */
    logger.info("update")<<"using "<<pointsToAdd.size()<<" points";
    if(pointsToAdd.size() == 0){
        return;
    }
    Eigen::MatrixX2d input(2,pointsToAdd.size());
    for(int row = 0; row < (int)pointsToAdd.size(); row++){
        input.col(row) = Eigen::Vector3d(pointsToAdd[row].x,pointsToAdd[row].y,weights[row]);
    }
    lineX->update(input);
    /*
    for(int i = 0; i < lineX->state.rows()*lineX->state.cols(); i++){
        if(std::isnan(lineX->state(i))){
            logger.error("update")<<"AFTER UPDATE: state is nan, index: "<<i;
            exit(0);
        }
    }
    */
    //lineX->state(lineX->state.rows()-1) = lineX->state(lineX->state.rows()-2);
    pointsAdded = pointsToAdd;
    pointsToAdd.clear();
    weights.clear();
}

std::vector<lms::math::vertex2f> LocalCourse::getPointsToAdd(){
    return pointsToAdd;
}

std::vector<lms::math::vertex2f> LocalCourse::getPointsAdded(){
    return pointsAdded;
}


void LocalCourse::resetData(){
    //resetCounter = 0;
    pointsToAdd.clear();
    pointsAdded.clear();
    weights.clear();
    configsChanged();
}


bool LocalCourse::addPoints(const std::vector<lms::math::vertex2f> &points){
    bool ret = true;
    for(const lms::math::vertex2f &v :points){
        ret = addPoint(v) && ret;
    }
    return ret;
}

bool LocalCourse::addPoints(const std::vector<lms::math::vertex2f> &points,const std::vector<float> &weights){
    bool ret = true;
    if(points.size() != weights.size()){
        logger.error("addPoints")<<"points.size() != weights.size()";
        return false;
    }
    for(std::size_t i = 0; i < points.size(); i++){
        ret = addPoint(points[i],weights[i]) && ret;
    }
    return ret;
}

bool LocalCourse::addPoint(const lms::math::vertex2f &p,const float &weight){
    if (!(std::isnan(p.x) || std::isnan(p.y))) {
        pointsToAdd.push_back(p);
        weights.push_back(weight);
        return true;
    }else{
        logger.error("addPoint") <<"point is nan!";
        return false;
    }
}

street_environment::RoadLane LocalCourse::getCourse(){
    Eigen::Matrix<double,Eigen::Dynamic,2> data = lineX->toXY();

    street_environment::RoadLane r;
    for(int row = 0; row < data.rows(); row++){
        r.points().push_back(lms::math::vertex2f(data(row,0),data(row,1)));
    }
    return r;
}


street_environment::RoadLane LocalCourse::getCourse(lms::Time time){
    (void)time;
    logger.error("DOES NOT WORK");
    LMS_EXCEPTION("NOT IMPLEMENTED YET!");
    //TODO
    street_environment::RoadLane r;
    return r;
}

} // namespace local_course

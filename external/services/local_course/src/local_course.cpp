#include "local_course/local_course.h"

namespace local_course {


LocalCourse::LocalCourse():kalman(logger){

}

bool LocalCourse::init() {
    kalman.configsChanged(config());
    return true;
}

void LocalCourse::destroy() {
    //TODO
}


void LocalCourse::update(float dx, float dy, float dphi){
    kalman.update(pointsToAdd,dx,dy,dphi);
    pointsToAdd.clear();
}

std::vector<lms::math::vertex2f> LocalCourse::getPointsToAdd(){
    return pointsToAdd;
}


void LocalCourse::resetData(){
    pointsToAdd.clear();
    kalman.resetData(config());
}


void LocalCourse::addPoints(const std::vector<lms::math::vertex2f> &points){
    for(const lms::math::vertex2f &v :points){
        pointsToAdd.push_back(v);
    }
}

void LocalCourse::addPoint(const lms::math::vertex2f &p){
    pointsToAdd.push_back(p);
}



street_environment::RoadLane LocalCourse::getCourse(){
    return kalman.getOutput();
}


street_environment::RoadLane LocalCourse::getCourse(lms::Time time){
    (void)time;
    //TODO
}

} // namespace local_course

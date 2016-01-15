#include "local_course/local_course.h"

namespace local_course {

bool LocalCourse::init() {
    return true;
}

void LocalCourse::destroy() {
}


void LocalCourse::update(float d, float dy, float dphi){

}


void LocalCourse::addPoints(const std::vector<lms::math::vertex2f> &points){
    for(const lms::math::vertex2f &v :points){
        pointsToAdd.push_back(v);
    }
}

void LocalCourse::addPoint(const lms::math::vertex2f &p){
    pointsToAdd.push_back(p);
}


void getCourse(){

}

void getCourse(lms::Time time){
    (void)time;
    //TODO
}

} // namespace local_course

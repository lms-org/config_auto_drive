#include "trajectory_point_creator.h"
#include "lms/datamanager.h"

bool TrajectoryPointCreator::initialize() {
   toFollow = datamanager()->readChannel<lms::math::polyLine2f>(this,"LINE");
   trajectoryPoint = datamanager()->writeChannel<lms::math::vertex<4,float>>(this,"POINT");
   config = getConfig();
   return true;
}

bool TrajectoryPointCreator::deinitialize() {
    return true;
}

bool TrajectoryPointCreator::cycle() {
    const float distanceSearched = config->get<float>("distanceSearched", 0.50);

    //logger.debug("cycle") << "#######START#######";
    bool found = false;
    for(int i = 1; i < (int)toFollow->points().size();i++){
        //TODO put 0.2 in config
        float length = toFollow->points()[i].length();
        //logger.debug("cycle") << "pos: " << toFollow->points()[i].x() << " " << toFollow->points()[i].y();
        if(length > distanceSearched){
            //logger.debug("cycle") << "FOUND: pos: " << toFollow->points()[i].x() << " " << toFollow->points()[i].y();
            float angle = toFollow->points()[i].angle();
            //x-Pos
            (*trajectoryPoint)[0] = distanceSearched*cos(angle);
            //y-Pos
            (*trajectoryPoint)[1] = distanceSearched*sin(angle);
            float dirAngle = toFollow->points()[i].angle(toFollow->points()[i-1]);
            if(i+1 < (int)toFollow->points().size()){
                dirAngle = lms::math::limitAngle_0_2PI(dirAngle)+lms::math::limitAngle_0_2PI(toFollow->points()[i].angle(toFollow->points()[i+1]));
                dirAngle = lms::math::limitAngle_0_2PI(dirAngle);
                dirAngle *= 0.5;
            }
            //x-Dir
            (*trajectoryPoint)[2] = cos(dirAngle);
            //y-Dir
            (*trajectoryPoint)[3] = sin(dirAngle);
            found = true;
            break;
        }
    }
    if(!found){
        //if we find nothing, we just want to drive forward
        //x-Pos
        (*trajectoryPoint)[0] = distanceSearched;
        //y-Pos
        (*trajectoryPoint)[1] = 0;
        //x-Dir
        (*trajectoryPoint)[2] = 1;
        //y-Dir
        (*trajectoryPoint)[3] = 0;
    }
    return true;
}


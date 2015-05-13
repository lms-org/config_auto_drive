#include "trajectory_point_creator.h"
#include "image_objects/environment.h"
#include "lms/math/polyline.h"
#include "lms/math/math.h"
#include "lms/math/interpolation.h"

bool TrajectoryPointCreator::initialize() {
   toFollow = datamanager()->readChannel<lms::math::polyLine2f>(this,"LINE");
   trajectoryPoint = datamanager()->writeChannel<lms::math::vertex<4,float>>(this,"TRAJECTORY_POINT");
   return true;
}

bool TrajectoryPointCreator::deinitialize() {
    return true;
}

bool TrajectoryPointCreator::cycle() {
    for(int i = 1; i < (int)toFollow->points().size();i++){
        //TODO put 0.2 in config
        float length = toFollow->points()[i].length();
        if(length > 0.2){
            float angle = toFollow->points()[i].angle();
            //x-Pos
            (*trajectoryPoint)[0] = 0.2*cos(angle);
            //y-Pos
            (*trajectoryPoint)[1] = 0.2*sin(angle);
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
        }
    }
    return true;
}


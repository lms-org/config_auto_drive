#include "trajectory_point_creator.h"
#include "lms/datamanager.h"

bool TrajectoryPointCreator::initialize() {
   toFollow = datamanager()->readChannel<lms::math::polyLine2f>(this,"LINE");
   trajectoryPoint = datamanager()->writeChannel<std::pair<lms::math::vertex2f,lms::math::vertex2f>>(this,"POINT");
   config = getConfig();
   return true;
}

bool TrajectoryPointCreator::deinitialize() {
    return true;
}

bool TrajectoryPointCreator::cycle() {
    const float distanceSearched = config->get<float>("distanceSearched", 0.50);

    bool found = false;
    for(int i = 1; i < (int)toFollow->points().size();i++){
        //TODO put 0.2 in config
        lms::math::vertex2f top = toFollow->points()[i];
        float length = top.length();
        //logger.debug("cycle") << "pos: " << toFollow->points()[i].x() << " " << toFollow->points()[i].y();
        if(length > distanceSearched){
            //TODO has to be tested
            //We start at the bottom-point
            lms::math::vertex2f bot = toFollow->points()[i-1];
            float diffLength = distanceSearched-bot.length();
            if(diffLength  < 0){
                diffLength = 0;
                bot.x = distanceSearched*cos(bot.angle());
                bot.y = distanceSearched*sin(bot.angle());
            }

            float angle = top.angle();
            //we to the bot-coords the length, that is still to go in absolute direction to 0,0
            //TODO point isn't on the trajectory but I think it could be fine
            //x-Pos
            trajectoryPoint->first.x = bot.x + diffLength*cos(angle);
            //y-Pos
            trajectoryPoint->first.y = bot.y + diffLength*sin(angle);

            //trying to reduce the error...
            float dirAngle = lms::math::limitAngle_0_2PI(toFollow->points()[i-1].angle(toFollow->points()[i]));

            /*
            if(i+1 < (int)toFollow->points().size()){
                float dirAngle2 = lms::math::limitAngle_0_2PI(toFollow->points()[i].angle(toFollow->points()[i+1]));

                //logger.error("dirAngle: ")<<dirAngle<<" dirAngle2: " <<dirAngle2;
                dirAngle = dirAngle+dirAngle2;
                dirAngle = lms::math::limitAngle_0_2PI(dirAngle);
                dirAngle *= 0.5;
            }
            */
            //x-Dir
            trajectoryPoint->second.x = cos(dirAngle);
            //y-Dir
            trajectoryPoint->second.y = sin(dirAngle);
            found = true;
            //logger.error("trajecPoint") <<(*trajectoryPoint)[0]<< " " << (*trajectoryPoint)[1] << " "<< (*trajectoryPoint)[2] <<" "<<(*trajectoryPoint)[3];
            break;
        }
    }
    if(!found){
        //if we find nothing, we just want to drive forward
        //x-Pos
        trajectoryPoint->first.x = distanceSearched;
        //y-Pos
        trajectoryPoint->first.y = 0;
        //x-Dir
        trajectoryPoint->second.x = 1;
        //y-Dir
        trajectoryPoint->second.y = 0;
    }
    return true;
}


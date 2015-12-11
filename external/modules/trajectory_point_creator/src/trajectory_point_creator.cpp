#include "trajectory_point_creator.h"

bool TrajectoryPointCreator::initialize() {
   toFollow = readChannel<lms::math::polyLine2f>("LINE");
   trajectoryPoint = writeChannel<street_environment::TrajectoryPoint>("POINT");
   return true;
}

bool TrajectoryPointCreator::deinitialize() {
    return true;
}

bool TrajectoryPointCreator::cycle() {
    const float distanceSearched = config().get<float>("distanceSearched", 0.50);

    bool found = false;
    if(toFollow->points().size() < 1){
        logger.warn("cycle") <<"Can't follow anything";
    }
    for(int i = 1; i < (int)toFollow->points().size();i++){
        //TODO put 0.2 in config
        lms::math::vertex2f top = toFollow->points()[i];
        //logger.debug("cycle") << "pos: " << toFollow->points()[i].x() << " " << toFollow->points()[i].y();
        if(top.x > distanceSearched){
            //TODO has to be tested
            //We start at the bottom-point
            lms::math::vertex2f bot = toFollow->points()[i-1];
            float toGoX = distanceSearched-bot.x;
            if(toGoX <= 0){
                //TODO
                toGoX = 0;
            }

            float angle = (top-bot).angle();
            //we to the bot-coords the length, that is still to go in absolute direction to 0,0
            //TODO point isn't on the trajectory but I think it could be fine
            //x-Pos
            trajectoryPoint->position.x = bot.x + toGoX*cos(angle);
            //y-Pos
            trajectoryPoint->position.y = bot.y + toGoX*sin(angle);

            //x-Dir
            trajectoryPoint->directory.x = cos(angle);
            //y-Dir
            trajectoryPoint->directory.y = sin(angle);
            found = true;
            break;
        }
    }
    if(!found){
        //if we find nothing, we just want to drive forward
        //x-Pos
        trajectoryPoint->position.x = distanceSearched;
        //y-Pos
        trajectoryPoint->position.y = 0;
        //x-Dir
        trajectoryPoint->directory.x = 1;
        //y-Dir
        trajectoryPoint->directory.y = 0;
    }
    return true;
}


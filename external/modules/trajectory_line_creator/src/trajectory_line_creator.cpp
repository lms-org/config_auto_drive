#include "trajectory_line_creator.h"
#include "lms/datamanager.h"

bool TrajectoryLineCreator::initialize() {
    environment = datamanager()->readChannel<Environment>(this,"ENVIRONMENT");
    line = datamanager()->writeChannel<lms::math::polyLine2f>(this,"LINE");
   return true;
}

bool TrajectoryLineCreator::deinitialize() {
    return true;
}

bool TrajectoryLineCreator::cycle() {
    if(environment->lanes.size() ==  0){
        logger.debug("cycle") << "no valid environment given";
        return true;
    }
    line->points() = environment->lanes[0].points();
    //move to line
    //reove points that are smaller than 0
    line->reduce([](const lms::math::vertex2f& p1){
        return p1.x() < 0;
    });
    line->move(lms::math::vertex2f(0,-0.2));
    return true;
}


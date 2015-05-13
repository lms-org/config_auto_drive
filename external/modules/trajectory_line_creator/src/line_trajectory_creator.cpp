#include "line_trajectory_creator.h"
#include "image_objects/environment.h"
#include "lms/math/polyline.h"

bool TrajectoryLineCreator::initialize() {
    raw_line = datamanager()->readChannel<Environment::RoadLane>(this,"LANE_MERGED");
    line = datamanager()->writeChannel<lms::math::polyLine2f>(this,"CONTROLL_LINE");
   return true;
}

bool TrajectoryLineCreator::deinitialize() {
    return true;
}

bool TrajectoryLineCreator::cycle() {
    line->points()=raw_line->points();
    //move to line
    line->move(lms::math::vertex2f(0,20));
    return true;
}


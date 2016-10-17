#include "socket_datatype_definer.h"
#include "lms/math/polyline.h"
#include "street_environment/trajectory.h"
#include "street_environment/road.h"
#include "street_environment/street_environment.h"
#include "street_environment/car.h"

bool SocketDatatypeDefiner::initialize() {
    readChannel<street_environment::Trajectory>("TRAJECTORY_LINE"); //TODO change it to trajectory
    readChannel<street_environment::TrajectoryPoint>("TRAJECTORY_POINT");
    readChannel<street_environment::Car>("CAR");
    readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_OBSTACLE");
    readChannel<street_environment::RoadLane>("MIDDLE_LANE");
    readChannel<lms::math::polyLine2f>("URG_DATA");
    return true;
}

bool SocketDatatypeDefiner::deinitialize() {
    return true;
}

bool SocketDatatypeDefiner::cycle() {
    return true;
}

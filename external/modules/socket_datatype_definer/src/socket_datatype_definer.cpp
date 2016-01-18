#include "socket_datatype_definer.h"
#include "lms/math/polyline.h"
#include "street_environment/trajectory.h"
#include "street_environment/road.h"
#include "street_environment/street_environment.h"
#include "sensor_utils/car.h"

bool SocketDatatypeDefiner::initialize() {
    readChannel<street_environment::Trajectory>("TRAJECTORY_LINE"); //TODO change it to trajectory
    readChannel<street_environment::TrajectoryPoint>("TRAJECTORY_POINT");
    readChannel<sensor_utils::Car>("CAR");
    readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_OBSTACLE");
    readChannel<street_environment::RoadLane>("MIDDLE_LANE");
    return true;
}

bool SocketDatatypeDefiner::deinitialize() {
    return true;
}

bool SocketDatatypeDefiner::cycle() {
    return true;
}

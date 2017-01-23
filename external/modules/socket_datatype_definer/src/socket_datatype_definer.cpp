#include "socket_datatype_definer.h"
#include "lms/math/polyline.h"
#include "street_environment/trajectory.h"
#include "street_environment/road.h"
#include "street_environment/street_environment.h"
#include "street_environment/car.h"
#include "street_environment/roadmatrix.h"
#include "lms/math/point_cloud.h"

bool SocketDatatypeDefiner::initialize() {
    readChannel<street_environment::Trajectory>("TRAJECTORY_LINE"); //TODO change it to trajectory
    readChannel<street_environment::TrajectoryPoint>("TRAJECTORY_POINT");
    readChannel<street_environment::CarCommand>("CAR");
    readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_MASTER");
    readChannel<street_environment::RoadLane>("MIDDLE_LANE");
    readChannel<lms::math::polyLine2f>("NEGATIVE_CENTER_LINE");
    readChannel<street_environment::RoadMatrix>("ROADMATRIX");
    readChannel<lms::math::PointCloud2f>("HOKUYO_LIDAR_DATA");
    readChannel<lms::math::PointCloud2f>("CULLED_HOKUYO_LIDAR_DATA");
    return true;
}

bool SocketDatatypeDefiner::deinitialize() {
    return true;
}

bool SocketDatatypeDefiner::cycle() {
    return true;
}

#include "socket_datatype_definer.h"
#include "lms/math/polyline.h"
#include "street_environment/trajectory.h"
#include "sensor_utils/car.h"

bool SocketDatatypeDefiner::initialize() {
    readChannel<street_environment::Trajectory>("TRAJECTORY_LINE"); //TODO change it to trajectory
    readChannel<street_environment::TrajectoryPoint>("TRAJECTORY_POINT");
    readChannel<sensor_utils::Car>("CAR");
    //datamanager()->readChannel<std::pair<lms::math::vertex2f,lms::math::vertex2f>(this,"TRAJECTORY_POINT");
    return true;
}

bool SocketDatatypeDefiner::deinitialize() {
    return true;
}

bool SocketDatatypeDefiner::cycle() {
    return true;
}

#include "socket_datatype_definer.h"
#include "lms/math/polyline.h"
#include "sensor_utils/car.h"

bool SocketDatatypeDefiner::initialize() {
    datamanager()->readChannel<lms::math::polyLine2f>(this,"TRAJECTORY_LINE");
    datamanager()->readChannel<sensor_utils::Car>(this,"CAR");
    //datamanager()->readChannel<std::pair<lms::math::vertex2f,lms::math::vertex2f>(this,"TRAJECTORY_POINT");
    return true;
}

bool SocketDatatypeDefiner::deinitialize() {
    return true;
}

bool SocketDatatypeDefiner::cycle() {
    return true;
}

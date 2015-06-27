#include <cmath>
#include <algorithm>
#include "car_to_matlab.h"
#include "lms/datamanager.h"
#include "sensor_utils/car.h"

bool CarToMatlab::initialize() {
    car = datamanager()->readChannel<sensor_utils::Car>(this,"CAR");
    return true;
}


bool CarToMatlab::deinitialize() {
    return true;
}

bool CarToMatlab::cycle() {
    return true;
}

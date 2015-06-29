#include "car_to_image.h"

extern "C" {

void* getInstance() {
    return new CarToImage();
}
}

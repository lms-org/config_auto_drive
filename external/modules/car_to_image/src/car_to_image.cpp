#include <cmath>
#include <algorithm>
#include "car_to_image.h"
#include "lms/imaging/graphics.h"
#include "sensor_utils/car.h"

bool CarToImage::initialize() {
    car = datamanager()->readChannel<sensor_utils::Car>(this,"CAR");
    image = datamanager()->writeChannel<lms::imaging::Image>(this, "IMAGE");
    int imageWidth=1024;
    int imageHeight=1024;

    meterToPxl = 50;
    offsetX = 200;
    offsetY = 800;
    image->resize(imageWidth,imageHeight,lms::imaging::Format::BGRA);
    image->fill(255);
    return true;
}


bool CarToImage::deinitialize() {
    return true;
}

bool CarToImage::cycle() {
    int x=offsetX+car->position().x*meterToPxl;
    int y=offsetY+car->position().y*meterToPxl;
    //TODO resize image so x,y are inside!
    logger.debug("cycle")<<"position: "<< x <<" "<<y;
    lms::imaging::BGRAImageGraphics graphics(*image);
    graphics.setColor(lms::imaging::blue);
    graphics.drawCross(x,y,4);
    return true;
}

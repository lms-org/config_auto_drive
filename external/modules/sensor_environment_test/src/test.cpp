#include <sensor_environment/test.h>
#include <lms/imaging/image.h>
#include <lms/datamanager.h>
#include <lms/imaging/graphics.h>
bool EnvTest::cycle(){
    return true;
}

bool EnvTest::initialize(){

    lde = new LinedDepthEnvironment(0.5,0.5);

    lde->add(1,0);
    lde->add(1,0);
    lde->add(1,2);
    lde->add(1,2);
    lde->add(1,2);
    lde->add(1,2);
    lde->add(1,4);
    lde->add(1,4);
    lde->add(1,1);
    lde->add(1,1);
    lde->add(1,4);
    lde->add(1,1);

    //lde->add(20,0);
    lde->validate();
    lms::imaging::Image *imagePtr = datamanager()->writeChannel<lms::imaging::Image>(this, "CAMERA_IMAGE");
    imagePtr->resize(1000, 1000, lms::imaging::Format::BGRA);
    imagePtr->fill(255);  // everything white
    logger.info("I AM HERE");
    lms::imaging::BGRAImageGraphics g(*imagePtr);
    lde->draw(&g,20);
    logger.info("AFTER DRAW HERE");
    return true;
}

bool EnvTest::deinitialize(){
    return false;
}

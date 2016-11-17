#include "car_track_visulizer.h"

bool CarTrackVisulizer::initialize() {
    window = new MainWindow();
    car = readChannel<street_environment::Car>("CAR");

    return true;
}

bool CarTrackVisulizer::deinitialize() {
    return true;
}

bool CarTrackVisulizer::cycle() {
    if(!window->isVisible())
        window->show();
    window->addPoint(car->position().x,car->position().y,car->viewDirection().angle());
    window->addVelocity(car->velocity(),car->targetSpeed());//velocity()); TODO
    window->addSteering(car->steeringFront()*180/M_PI,car->steeringRear()*180/M_PI);
    return true;
}

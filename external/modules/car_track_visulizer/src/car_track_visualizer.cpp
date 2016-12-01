#include "car_track_visualizer.h"

bool CarTrackVisualizer::initialize() {
    window = new MainWindow();
    car = readChannel<street_environment::Car>("CAR");
    car_ground_truth = readChannel<street_environment::Car>("CAR_GROUND_TRUTH");

    return true;
}

bool CarTrackVisualizer::deinitialize() {
    return true;
}

bool CarTrackVisualizer::cycle() {
    if(!window->isVisible())
        window->show();
    logger.error("position")<<car->position().x<<" "<<car->position().y;
    window->addPoint(car->position().x,car->position().y,car->viewDirection().angle());
    window->addVelocity(car->velocity(),car->targetSpeed());//velocity()); TODO
    window->addSteering(car->steeringFront()*180/M_PI,car->steeringRear()*180/M_PI);
    //TODO hasWriter if(car_ground_truth.)
    return true;
}

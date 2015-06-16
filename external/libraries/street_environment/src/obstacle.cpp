#include "street_environment/obstacle.h"

namespace street_environment{
Obstacle::Obstacle() : position(0, 0), viewDirection(1, 0),
    velocity(0), moveDirection(1, 0), lastPositon(0, 0), lastVelocity(0) {}

void Obstacle::updatePosition(const lms::math::vertex2f &position,
                                   const lms::math::vertex2f &viewDirection) {
    this->lastPositon = this->position;
    this->position = position;
    this->viewDirection = viewDirection;
}

void Obstacle::updateVelocity(float velocity,
                    const lms::math::vertex2f &moveDirection) {
    this->lastVelocity = this->velocity;
    this->velocity = velocity;
    this->moveDirection = moveDirection;
}

float Obstacle::movedDistance() const {
    return this->position.distance(this->lastPositon);
}

float Obstacle::deltaVelocity() const {
    return this->velocity - this->lastVelocity;
}
} //street_environment

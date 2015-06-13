#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include "lms/math/vertex.h"
#include "street_environment.h"

namespace street_environment{
/**
 * @brief A dynamic entity can be the vehicle itself but also every other
 * moving obstacle.
 */
struct Obstacle:public EnvironmentObject {
    //TODO die Delta-Werte passen beim 1. Aufrufen nicht!
    /**
     * @brief Global position of the entity. x and y are given in meters.
     */
    lms::math::vertex2f position;

    /**
     * @brief Direction vector that points to the direction the entity is
     * looking at.
     */
    lms::math::vertex2f viewDirection;

    /**
     * @brief Velocity in m/s of the entity.
     */
    float velocity;

    /**
     * @brief Direction vector that points to the direction the entity is
     * driving to.
     */
    lms::math::vertex2f moveDirection;

    /**
     * @brief Global position of the entity in the last cycle.
     */
    lms::math::vertex2f lastPositon;

    /**
     * @brief Velocity of the entity in the last cycle.
     */
    float lastVelocity;

    Obstacle();

    /**
     * @brief Set the position for the current cycle. Should be called only
     * once per cycle.
     * @param position global position of the entity
     * @param viewDirection direction vector of the view of the entity
     */
    void updatePosition(const lms::math::vertex2f &position,
                        const lms::math::vertex2f &viewDirection);

    /**
     * @brief Set the velocity for the current cycle. Should be called only
     * once per cycle.
     * @param velocity velocity of the entity
     * @param moveDirection direction vector of the movement of the entity
     */
    void updateVelocity(float velocity,
                        const lms::math::vertex2f &moveDirection);

    /**
     * @brief Distance travelled compared to the last cycle.
     * @return distance in m
     */
    float movedDistance() const;

    /**
     * @brief Difference in velocity to the last cycle.
     * @return delta velocity in m/s
     */
    float deltaVelocity() const;
};
} //street_environment

#endif /* DYNAMIC_ENTITY_H */

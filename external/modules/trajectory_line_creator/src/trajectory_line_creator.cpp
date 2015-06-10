#include "trajectory_line_creator.h"
#include "lms/datamanager.h"
#include "lms/math/math.h"

bool TrajectoryLineCreator::initialize() {
    environment = datamanager()->readChannel<street_environment::Environment>(this,"ENVIRONMENT");
    line = datamanager()->writeChannel<lms::math::polyLine2f>(this,"LINE");
    config = getConfig();
    return true;
}

bool TrajectoryLineCreator::deinitialize() {
    return true;
}

bool TrajectoryLineCreator::cycle() {
    // translate the middle lane to the right with a quarter of the street width
    const float translation = 4.0f / config->get<float>("street.width", 0.8);

    using lms::math::vertex2f;
    if(environment->objects.size() ==  0){
        logger.debug("cycle") << "no valid environment given";
        return true;
    }
    line->points().clear();
    //line->points() = environment->lanes[0].points();
    //move to line
    //reove points that are smaller than 0

    //TODO write method for that
    const street_environment::RoadLane &middle = environment->objects[0]->getAsReference<const street_environment::RoadLane>();
    for(size_t i = 1; i < middle.points().size(); i++) {
        vertex2f p1 = middle.points()[i - 1];
        vertex2f p2 = middle.points()[i];
        if(p1 == p2)
            continue;

        vertex2f along = p2 - p1;
        vertex2f mid((p1.x + p2.x) / 2., (p1.y + p2.y) / 2.);
        vertex2f normAlong = along / along.length();
        vertex2f orthogonal(normAlong.y, -normAlong.x);
        orthogonal = orthogonal / translation;
        vertex2f result = mid + orthogonal;
        line->points().push_back(result);
    }

    line->reduce([](const lms::math::vertex2f& p1){
        return p1.x < 0;
    });

    return true;
}


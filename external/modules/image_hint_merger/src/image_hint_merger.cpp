#include "image_hint_merger.h"
#include "lms/imaging/find/line.h"
#include "lms/imaging/warp.h"

#include <algorithm>

bool ImageObjectMerger::initialize() {

    input = datamanager()->readChannel<Environment>(this, "ENVIRONMENT_INPUT");
    output = datamanager()->writeChannel<Environment>(this, "ENVIRONMENT_OUTPUT");
    return true;
}

bool ImageObjectMerger::deinitialize() {
    return true;
}

bool ImageObjectMerger::cycle() {
    using lms::math::vertex2f;

    // TODO just for testing, later on there has to be some config that states
    // what ImageObjects of what ENVIRONMENT should be merged

    Environment::RoadLane middleLane;
    middleLane.type(Environment::RoadLaneType::MIDDLE);

    // move rigth and left lane points to middle lane
    for(const Environment::RoadLane &lane : input->lanes) {
        if(lane.type() == Environment::RoadLaneType::LEFT) {
            transform(lane, middleLane, 1);
        } else if(lane.type() == Environment::RoadLaneType::RIGHT) {
            transform(lane, middleLane, -1);
        }
    }

    std::sort(middleLane.points().begin(), middleLane.points().end(),
              [](vertex2f p1, vertex2f p2) {
        return p1.x() < p2.x();
    });

    output->lanes.clear();
    output->lanes.push_back(middleLane);

    return true;
}

void ImageObjectMerger::transform(const Environment::RoadLane &fromLane,
                                  Environment::RoadLane &toLane, int sign) {
    using lms::math::vertex2f;

    for(size_t i = 1; i < fromLane.points().size(); i++) {
        vertex2f p1 = fromLane.points()[i - 1];
        vertex2f p2 = fromLane.points()[i];

        vertex2f along = p2 - p1;
        vertex2f mid((p1.x() + p2.x()) / 2., (p1.y() + p2.y()) / 2.);

        vertex2f normAlong = along / along.length();
        vertex2f orthogonal(sign * (-1) * normAlong.y(), sign * normAlong.x());

        orthogonal = orthogonal / 50;

        vertex2f result = mid + orthogonal;

        toLane.points().push_back(result);
    }
}

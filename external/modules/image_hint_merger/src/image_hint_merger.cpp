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
            transform(lane, middleLane, -1);
        } else if(lane.type() == Environment::RoadLaneType::RIGHT) {
            transform(lane, middleLane, +1);
        }
    }

    std::sort(middleLane.points().begin(), middleLane.points().end(),
              [](vertex2f p1, vertex2f p2) {
        return p1.x() < p2.x();
    });
    output->lanes.clear();
    prepareLane(middleLane,true,true);
    output->lanes.push_back(middleLane);

    return true;
}

void ImageObjectMerger::prepareLane(Environment::RoadLane &lane, bool checkAngle, bool checkDistance){
    using lms::math::vertex2f;
    lane.sort([](const vertex2f &p1,const vertex2f &p2) {
        return p1.x() < p2.x();
    });

    if(checkAngle){
        float lastAngle = INFINITY;
        float lastAngleChange = INFINITY;
        //TODO we could work with the change of the angle
        //TODO we could also use the length of between those points!
        lane.reduce([&lastAngle](const vertex2f& p1,const vertex2f& p2){
            //TODO 0.1 should be moved to config
            float currentAngle = p2.angle(p1);
            bool remove = false;
            if(lastAngle != INFINITY){
                //we are looking for the smaller angle
                float deltaAngle = abs(lastAngle-currentAngle);
                if(deltaAngle > M_PI){
                    deltaAngle = M_PI_2-deltaAngle;
                }
                //TODO 30 should be moved into a config
                remove = (deltaAngle > 10.0/180.0*M_PI);
            }
            lastAngle = currentAngle;

            return remove;
        });
    }
    if(checkDistance){
        lane.reduce([](const vertex2f& p1,const vertex2f& p2){
            //TODO 0.1 should be moved to config
            bool remove = p1.distance(p2) < 0.1;
            return remove;
        });
    }

}

void ImageObjectMerger::transform(const Environment::RoadLane &fromLane,
                                  Environment::RoadLane &toLane, int sign) {
    using lms::math::vertex2f;
    //sort x-values
    Environment::RoadLane toSort = fromLane;
    prepareLane(toSort,true,false);


    for(size_t i = 1; i < toSort.points().size(); i++) {
        vertex2f p1 = toSort.points()[i - 1];
        vertex2f p2 = toSort.points()[i];

        logger.debug() <<"########################";
        logger.debug() << "P1: " <<p1.x() << " " << p1.y();
        logger.debug() << "P2: " <<p2.x() << " " << p2.y();

        vertex2f along = p2 - p1;
        logger.debug() << "ALONG: " <<along.x() << " " << along.y();
        vertex2f mid((p1.x() + p2.x()) / 2., (p1.y() + p2.y()) / 2.);
        logger.debug() << "MID: " <<mid.x() << " " << mid.y();

        vertex2f normAlong = along / along.length();
        logger.debug() << "NormAlong: " <<normAlong.x() << " " << normAlong.y();
        vertex2f orthogonal(sign * (-1) * normAlong.y(), sign * normAlong.x());
        logger.debug() << "ortho: " <<orthogonal.x() << " " << orthogonal.y();
        orthogonal = orthogonal / 2.5;
        logger.debug() << "verschiebe: "<<orthogonal.x() << " "<< orthogonal.y();
        vertex2f result = mid + orthogonal;
        logger.debug() << "res: " <<result.x() << " " << result.y();

        toLane.points().push_back(result);
    }
}

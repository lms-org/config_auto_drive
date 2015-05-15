#include "image_hint_merger.h"
#include "lms/imaging/find/line.h"
#include "lms/imaging/warp.h"
#include <cmath>

#include <algorithm>

bool ImageObjectMerger::initialize() {

    input = datamanager()->readChannel<Environment>(this, "ENVIRONMENT_INPUT");
    output = datamanager()->writeChannel<Environment>(this, "ENVIRONMENT_OUTPUT");
    return true;
}


bool ImageObjectMerger::deinitialize() {
    return true;
}


bool outprint = false; //TODO
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
    //outprint = true;
    prepareLane(middleLane,true,true);
    outprint = false;
    output->lanes.push_back(middleLane);

    return true;
}

void ImageObjectMerger::prepareLane(Environment::RoadLane &lane, bool checkAngle, bool checkDistance){
    using lms::math::vertex2f;
    if(outprint){
        std::cout << "#################################" <<std::endl;
    }
    lane.sort([](const vertex2f &p1,const vertex2f &p2) {
        return p1.x() < p2.x();
    });

    if(checkAngle){
        float lastAngle = INFINITY;

        //Reduce with two points
        //float lastAngleChange = INFINITY;
        //TODO we could work with the change of the angle
        //TODO we could also use the length of between those points!
        lane.reduce([&lastAngle](const vertex2f& p1,const vertex2f& p2){
            //TODO 0.1 should be moved to config
            float currentAngle = p2.angle(p1);
            bool remove = false;
            if(lastAngle != INFINITY){
                //we are looking for the smaller angle
                float deltaAngle = fabs(lastAngle-currentAngle);
                if(deltaAngle > M_PI){
                    deltaAngle = M_PI_2-deltaAngle;
                }
                //TODO 30 should be moved into a config
                remove = (deltaAngle > 30.0/180.0*M_PI);
                if(outprint){
                    std::cout<<"lastAngle: "<<lastAngle<< " currentAngle: "<<currentAngle << " delta_Angle" << deltaAngle << " remove: "<<remove<<std::endl;
                }
            }
            if(!remove)
                lastAngle = currentAngle;

            return remove;
        });



        //use 3 points with angle (just for testing)
        lastAngle = INFINITY;
        lane.reduce([&lastAngle](const vertex2f& p1,const vertex2f& p2,const vertex2f& p3){
            //TODO 0.1 should be moved to config
            float currentAngle1 = p1.angle(p2);
            float currentAngle2 = p1.angle(p3);
            bool remove = false;
            if(lastAngle != INFINITY){
                //we are looking for the smaller angle
                float deltaAngle1 = fabs(lastAngle-currentAngle1);
                if(deltaAngle1 > M_PI){
                    deltaAngle1 = M_PI_2-deltaAngle1;
                }
                float deltaAngle2 = fabs(lastAngle-currentAngle2);
                if(deltaAngle2 > M_PI){
                    deltaAngle2 = M_PI_2-deltaAngle2;
                }
                //TODO 30 should be moved into a config
                remove = (fabs(deltaAngle2-deltaAngle1) > 30.0/180.0*M_PI);
                if(outprint){
                    std::cout<<"lastAngle: "<<lastAngle<< " currentAngle: "<<currentAngle1 << " delta_Angle" << deltaAngle1 << ","<<deltaAngle2 << " remove: "<<remove<<std::endl;
                }
            }
            if(!remove)
                lastAngle = currentAngle1;

            return remove;
        });

        //Reduce with 3 points
    }

    if(checkDistance){

        lane.reduce([](const vertex2f& p1,const vertex2f& p2,const vertex2f& p3){
            bool remove =  p1.distance(p3)-p1.distance(p2) < 0.05;
            return remove;
        });


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
    prepareLane(toSort,true,true);

    //TODO write method for that
    for(size_t i = 1; i < toSort.points().size(); i++) {
        vertex2f p1 = toSort.points()[i - 1];
        vertex2f p2 = toSort.points()[i];
        if(p1 == p2)
            continue;

        vertex2f along = p2 - p1;
        vertex2f mid((p1.x() + p2.x()) / 2., (p1.y() + p2.y()) / 2.);
        vertex2f normAlong = along / along.length();
        vertex2f orthogonal(sign * (-1) * normAlong.y(), sign * normAlong.x());
        orthogonal = orthogonal / 2.5;
        vertex2f result = mid + orthogonal;
        toLane.points().push_back(result);
    }
}

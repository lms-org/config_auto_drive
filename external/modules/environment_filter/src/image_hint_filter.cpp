#include "image_hint_filter.h"
#include <cmath>
#include <algorithm>
#include "lms/datamanager.h"

bool EnvironmentFilter::initialize() {
    input = datamanager()->readChannel<Environment>(this, "ENVIRONMENT_INPUT");
    output = datamanager()->writeChannel<Environment>(this, "ENVIRONMENT_OUTPUT");
    return true;
}


bool EnvironmentFilter::deinitialize() {
    return true;
}

bool EnvironmentFilter::cycle() {
    output->lanes.clear();
    Environment::RoadLane filtered;
    for(Environment::RoadLane lane : input->lanes){
        filtered = lane;
        filterLane(filtered,true,true);
        output->lanes.push_back(filtered);
    }
    return true;
}

void EnvironmentFilter::filterLane(Environment::RoadLane &lane, bool checkAngle, bool checkDistance){
    using lms::math::vertex2f;


    //remove points that are inside the car
    lane.reduce([this](const vertex2f& p1){
        //TODO 0.1 should be moved to config
        bool remove = std::abs(p1.x()) < 0.3 && std::abs(p1.y())<0.1;
        return remove;
    });

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

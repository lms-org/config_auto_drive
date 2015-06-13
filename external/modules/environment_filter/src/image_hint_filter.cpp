#include "image_hint_filter.h"
#include <cmath>
#include <algorithm>
#include "lms/datamanager.h"

bool EnvironmentFilter::initialize() {
    input = datamanager()->readChannel<street_environment::Environment>(this, "ENVIRONMENT_INPUT");
    output = datamanager()->writeChannel<street_environment::Environment>(this, "ENVIRONMENT_OUTPUT");
    return true;
}


bool EnvironmentFilter::deinitialize() {
    return true;
}

bool EnvironmentFilter::cycle() {
    //TODO destroy objects
    output->objects.clear();
    //street_environment::RoadLane filtered;
    for(const std::shared_ptr<street_environment::EnvironmentObject> &obj : input->objects){
        if(obj->name().find("LANE") != std::string::npos){
            std::shared_ptr<street_environment::RoadLane> lane = obj->getCopyAsPtr<street_environment::RoadLane>();
            filterLane(*lane.get());
            output->objects.push_back(lane);
        }else if(obj->name().find("OBSTACLE")!= std::string::npos){
            //TODO
            output->objects.push_back(obj);
        }
    }
    return true;
}

void EnvironmentFilter::filterLane(street_environment::RoadLane &lane){
    using lms::math::vertex2f;
    //remove points that are inside the car
    lane.reduce([this](const vertex2f& p1){
        //TODO 0.1 should be moved to config
        bool remove = std::abs(p1.x) < 0.3 && std::abs(p1.y)<0.1;
        remove = remove || p1.length() > 1.5;
        return remove;
    });
    //sort points (not sure if that is smart)
    lane.sort([](const vertex2f &p1,const vertex2f &p2) {
        return p1.x < p2.x;
    });
    //remove triangles
    lane.reduce([](const vertex2f& p1,const vertex2f& p2,const vertex2f& p3){
        bool remove =  p1.distance(p3)-p1.distance(p2) < 0.05;
        return remove;
    });
    //remove unnecessary points
    lane.reduce([](const vertex2f& p1,const vertex2f& p2){
        //TODO 0.1 should be moved to config
        bool remove = p1.distance(p2) < 0.05;
        return remove;
    });





    float lastAngle = INFINITY;
    bool removeAll = false;
    //Reduce with two points
    //float lastAngleChange = INFINITY;
    //TODO we could work with the change of the angle
    //TODO we could also use the length of between those points!
    lane.reduce([this,&lastAngle, &removeAll](const vertex2f& p1,const vertex2f& p2){
        if(removeAll){
            return true;
        }
        //TODO 0.1 should be moved to config
        float currentAngle = (p2 - p1).angle();
        bool remove = false;
        if(lastAngle != INFINITY){
            //we are looking for the smaller angle
            float deltaAngle = fabs(lastAngle-currentAngle);
            if(deltaAngle > M_PI){
                deltaAngle = deltaAngle-M_PI;
            }
            if(deltaAngle > 60.0/180.0*M_PI){
                logger.info("...") <<lastAngle << " " << currentAngle;
                logger.info("remove all") <<deltaAngle;
                removeAll = true;
            }
            //TODO 30 should be moved into a config
            remove = (deltaAngle > 20.0/180.0*M_PI);
        }
        if(!remove)
            lastAngle = currentAngle;

        return remove;
    });



    //use 3 points with angle (just for testing)
    lastAngle = INFINITY;
    lane.reduce([&lastAngle](const vertex2f& p1,const vertex2f& p2,const vertex2f& p3){
        //TODO 0.1 should be moved to config
        float currentAngle1 = (p2 - p1).angle();
        float currentAngle2 = (p3 - p1).angle();
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

}

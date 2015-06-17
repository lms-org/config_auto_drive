#include "trajectory_line_creator.h"
#include "lms/datamanager.h"
#include "lms/math/math.h"
#include "street_environment/obstacle.h"

bool TrajectoryLineCreator::initialize() {
    environment = datamanager()->readChannel<street_environment::Environment>(this,"ENVIRONMENT");
    environmentMiddle = datamanager()->readChannel<street_environment::Environment>(this,"ENVIRONMENT_MIDDLE");
    line = datamanager()->writeChannel<lms::math::polyLine2f>(this,"LINE");
    car = datamanager()->writeChannel<sensor_utils::Car>(this,"CAR");
    config = getConfig();

    return true;
}

bool TrajectoryLineCreator::deinitialize() {
    return true;
}
bool stayLeft = false;
float deltaPos = 0;
bool TrajectoryLineCreator::cycle() {

    // translate the middle lane to the right with a quarter of the street width
    const float translation = 4.0f / config->get<float>("street.width", 0.8);
    //TODO das sollte von der aktuellen geschwindigkeit abhängen!
    float distanceObstacleBeforeChangeLine = 0.4;

    using lms::math::vertex2f;
    if(environmentMiddle->objects.size() ==  0){
        logger.debug("cycle") << "no valid environment given";
        return true;
    }
    line->points().clear();

    //line->points() = environment->lanes[0].points();
    //move to line
    //remove points that are smaller than 0

    //TODO write method for that
    float obstacleDistance = INFINITY;
    for(std::shared_ptr<street_environment::EnvironmentObject> obj : environment->objects){
        if(obj->name().find("OBSTACLE") != std::string::npos){
            const street_environment::Obstacle &obst = obj->getAsReference<const street_environment::Obstacle>();
            if(obst.position.length()<obstacleDistance){
                if(obst.position.length() < obstacleDistance){
                    obstacleDistance = obst.position.length();
                }
            }
        }
    }
    const street_environment::RoadLane &middle = environmentMiddle->getObjectByName("MIDDLE_LANE")->getAsReference<const street_environment::RoadLane>();
    //TODO
    if(stayLeft){
        deltaPos += car->deltaPosition().length();
        logger.debug("stay left: STEP") <<deltaPos;
    }
    //
    for(size_t i = 1; i < middle.points().size(); i++) {
        vertex2f p1 = middle.points()[i - 1];
        vertex2f p2 = middle.points()[i];
        if(p1 == p2)
            continue;

        vertex2f along = p2 - p1;
        vertex2f mid((p1.x + p2.x) / 2., (p1.y + p2.y) / 2.);
        vertex2f normAlong = along / along.length();
        vertex2f orthogonal(normAlong.y, -normAlong.x);
        //TODO geht schief, wenn man dran vorbei fährt!
        if(obstacleDistance-mid.length() < distanceObstacleBeforeChangeLine){
            if(mid.length() < 0.6){
                stayLeft = true;
                logger.debug("stay left: BEGIN") <<deltaPos;
                deltaPos = 0;
            }
            orthogonal *= -1;
        }else if(stayLeft){
            orthogonal *= -1;

        }
        if(deltaPos > 1.5){
            stayLeft = false;
        }
        orthogonal = orthogonal / translation;
        vertex2f result = mid + orthogonal;
        line->points().push_back(result);
    }

    line->reduce([](const lms::math::vertex2f& p1){
        return p1.x < 0;
    });

    return true;
}


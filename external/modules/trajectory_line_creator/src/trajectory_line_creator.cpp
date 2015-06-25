#include "trajectory_line_creator.h"
#include "lms/datamanager.h"
#include "lms/math/math.h"
#include "street_environment/obstacle.h"

bool TrajectoryLineCreator::initialize() {
    envObstacles = datamanager()->readChannel<street_environment::EnvironmentObstacles>(this,"ENVIRONMENT_OBSTACLES");
    road = datamanager()->readChannel<street_environment::RoadLane>(this,"ROAD");
    trajectory = datamanager()->writeChannel<lms::math::polyLine2f>(this,"LINE");
    car = datamanager()->writeChannel<sensor_utils::Car>(this,"CAR");
    config = getConfig();

    return true;
}

bool TrajectoryLineCreator::deinitialize() {
    return true;
}
bool TrajectoryLineCreator::cycle() {

    //clear old trajectory
    trajectory->points().clear();

    // translate the middle lane to the right with a quarter of the street width
    const float translation = config->get<float>("street.width", 0.8)/4.0f;
    //TODO das sollte von der aktuellen geschwindigkeit abhängen!
    float distanceObstacleBeforeChangeLine = 0.4;

    using lms::math::vertex2f;
    if(road->points().size() == 0){
        logger.error("cycle") << "no valid environment given";
        return true;
    }

    const street_environment::RoadLane &middle = *road;

    for(size_t i = 1; i < middle.points().size(); i++) {
        vertex2f p1 = middle.points()[i - 1];
        vertex2f p2 = middle.points()[i];
        if(p1 == p2)
            continue;

        vertex2f along = p2 - p1;
        vertex2f mid((p1.x + p2.x) / 2., (p1.y + p2.y) / 2.);
        vertex2f normAlong = along / along.length();
        vertex2f orthogonal(normAlong.y, -normAlong.x);

        bool left = false;
        //man geht davon aus, dass die Abstand, in dem man ausweicht deutlich größer ist als das hinderniss lang!
        float obstacleLength = 0.3;
        //check all obstacles
        for(const std::shared_ptr<street_environment::Obstacle> obst : envObstacles->objects){
            float x = obst->position().x;
            float y= obst->position().y;
            if(x < 0){
                x += obstacleLength;
            }
            if(pow(x*x+y*y,0.5)-mid.length() < distanceObstacleBeforeChangeLine ){
                left = true;
                break;
            }
        }

        if(left){
            orthogonal *= -1;
        }
        orthogonal = orthogonal * translation;
        vertex2f result = mid + orthogonal;
        trajectory->points().push_back(result);
    }

    trajectory->reduce([](const lms::math::vertex2f& p1){
        return p1.x < 0;
    });

    return true;
}


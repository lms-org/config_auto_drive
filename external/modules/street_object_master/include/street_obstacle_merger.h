#pragma once

#include "lms/module.h"
#include "street_environment/car.h"
#include "street_environment/street_environment.h"
#include "street_environment/obstacle.h"
#include "street_environment/crossing.h"
#include "lms/math/math.h"
#include "lms/math/pose.h"

class StreetObjectMaster : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    /**
     * @brief merge adds new obstacles to the old once
     * @param obstaclesNew
     * @param obstaclesOld
     */
    void merge(street_environment::EnvironmentObstacles &obstaclesNew,street_environment::EnvironmentObstacles &obstaclesOld,const std::set<std::string> &allSensorIds);
    void createOutput(street_environment::EnvironmentObstacles &obstaclesOld);
    float distanceTang(street_environment::ObstaclePtr obstacle);
    float distanceOrth(street_environment::ObstaclePtr obstacle);


    bool inVisibleArea(const street_environment::ObstaclePtr obst);

    lms::Time lastUpdate;
    lms::math::Pose2D getDeltaPose();

    void getObstacles(const street_environment::EnvironmentObjects &env,street_environment::EnvironmentObstacles &output);
    std::vector<std::pair<lms::ReadDataChannel<street_environment::EnvironmentObjects>,std::set<std::string>>> envInput;
    lms::WriteDataChannel<street_environment::EnvironmentObjects> envOutput;
    lms::ReadDataChannel<street_environment::RoadLane> middle;
    lms::ReadDataChannel<lms::math::Pose2DHistory> poseHistory;

    void checkAngle(street_environment::ObstaclePtr obst);

};


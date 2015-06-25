#ifndef LMS_TRAJECTORY_POINT_CREATOR_H
#define LMS_TRAJECTORY_POINT_CREATOR_H

#include "lms/module.h"
#include "street_environment/street_environment.h"
#include "street_environment/obstacle.h"

class StreetObstacleMerger : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    void merge(std::vector<street_environment::Obstacle*> &obstaclesNew,std::vector<street_environment::Obstacle*> &obstaclesOld);
    void filter(std::vector<street_environment::Obstacle*> &obstaclesOld);
    void createOutput(std::vector<street_environment::Obstacle*> &obstaclesOld);


    void getObstacles(const street_environment::EnvironmentObjects &env,std::vector<street_environment::Obstacle*> &output);
    bool merge(street_environment::Obstacle &from, street_environment::Obstacle &to);
    const street_environment::EnvironmentObjects *envInput;
    street_environment::EnvironmentObjects *envOutput;


};

#endif /* LMS_TRAJECTORY_POINT_CREATOR_H */

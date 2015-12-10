#ifndef LMS_TRAJECTORY_POINT_CREATOR_H
#define LMS_TRAJECTORY_POINT_CREATOR_H

#include "lms/module.h"
#include "lms/math/polyline.h"
#include "lms/math/math.h"
#include "lms/data_channel.h"
#include "street_environment/trajectory.h"

class TrajectoryPointCreator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    lms::ReadDataChannel<lms::math::polyLine2f> toFollow;
    lms::WriteDataChannel<street_environment::TrajectoryPoint> trajectoryPoint;


};

#endif /* LMS_TRAJECTORY_POINT_CREATOR_H */

#ifndef LMS_TRAJECTORY_POINT_CREATOR_H
#define LMS_TRAJECTORY_POINT_CREATOR_H

#include "lms/module.h"
#include "lms/math/polyline.h"
#include "lms/math/math.h"
#include "lms/data_channel.h"

class TrajectoryPointCreator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    const lms::ModuleConfig *config;
    lms::ReadDataChannel<lms::math::polyLine2f> toFollow;
    lms::WriteDataChannel<std::pair<lms::math::vertex2f, lms::math::vertex2f>> trajectoryPoint;


};

#endif /* LMS_TRAJECTORY_POINT_CREATOR_H */

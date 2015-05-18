#ifndef LMS_TRAJECTORY_POINT_CREATOR_H
#define LMS_TRAJECTORY_POINT_CREATOR_H

#include "lms/module.h"
#include "lms/math/polyline.h"
#include "lms/math/math.h"

class TrajectoryPointCreator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    const lms::type::ModuleConfig *config;
    const lms::math::polyLine2f *toFollow;
    std::pair<lms::math::vertex2f, lms::math::vertex2f> *trajectoryPoint;


};

#endif /* LMS_TRAJECTORY_POINT_CREATOR_H */

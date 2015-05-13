#ifndef LMS_TRAJECTORY_POINT_CREATOR_H
#define LMS_TRAJECTORY_POINT_CREATOR_H

#include "lms/module.h"
#include "lms/datamanager.h"
#include "lms/imaging/find/image_hint.h"
#include "image_objects/environment.h"
#include "lms/math/polyline.h"

class TrajectoryPointCreator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    const lms::math::polyLine2f *toFollow;
    lms::math::vertex<4,float> *trajectoryPoint;


};

#endif /* LMS_TRAJECTORY_POINT_CREATOR_H */

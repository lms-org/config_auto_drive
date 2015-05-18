#ifndef IMAGE_HINT_TRANSFORMER_H
#define IMAGE_HINT_TRANSFORMER_H

#include "lms/module.h"
#include "lms/datamanager.h"
#include "lms/math/vertex.h"

#include "comm/senseboard.h"

class TrajectoryLineFollower : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    std::pair<float, float> TobisRegler(double x_s_n, double y_s_n, double phi_s, double v);
    std::pair<float,float> TobisRegler_Simpel(double x_s, double y_s, double phi_s, double v);
    std::pair<float,float> smartRegler(float x, float y, float phi);

    Comm::SensorBoard::ControlData *controlData;
    const std::pair<lms::math::vertex2f, lms::math::vertex2f> *trajectoryPoint;
};

#endif /* IMAGE_HINT_TRANSFORMER_H */

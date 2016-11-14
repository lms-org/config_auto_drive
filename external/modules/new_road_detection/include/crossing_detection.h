#ifndef PHX_CROSSING_DETECTION_H
#define PHX_CROSSING_DETECTION_H
#include <lms/module.h>
#include <lms/imaging/image.h>
#include <lms/imaging/transform_image.h>
#include <lms/math/polyline.h>

#include <street_environment/crossing.h>
#include <street_environment/start_line.h>

class CrossingDetection: public lms::Module {

    lms::ReadDataChannel<lms::imaging::Image> image;
    lms::WriteDataChannel<lms::imaging::Image> imageDebug;
    lms::WriteDataChannel<street_environment::EnvironmentObjects> env;
    //all points from the lidar
    lms::ReadDataChannel<lms::math::polyLine2f> points;
    bool renderDebugImage;
    lms::imaging::Homography homo;
    int threshold;

public:
    bool init() override;
    void destroy() override;
    bool cycle() override;
    void configsChanged() override;

    bool find();

};


#endif //PHX_CROSSING_DETECTION_H

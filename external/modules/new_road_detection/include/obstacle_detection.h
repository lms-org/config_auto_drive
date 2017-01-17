#pragma once
#include <lms/module.h>
#include <lms/imaging/image.h>
#include <lms/imaging/transform_image.h>
#include <lms/math/polyline.h>

#include <street_environment/crossing.h>
#include "lms/imaging_detection/street_crossing.h"
#include "lms/imaging_detection/street_obstacle.h"
#include <street_environment/start_line.h>
#include "lms/imaging/graphics.h"

class ObstacleDetection: public lms::Module {

    lms::ReadDataChannel<lms::imaging::Image> image;
    lms::WriteDataChannel<lms::imaging::Image> imageDebug;
    lms::WriteDataChannel<street_environment::EnvironmentObjects> env;
    bool renderDebugImage;
    lms::imaging::Homography homo;
    int threshold;
    lms::imaging::Image* gaussBuffer;

public:
    bool init() override;
    void destroy() override;
    bool cycle() override;
    void configsChanged() override;

    void find(const lms::imaging::detection::StreetObstacle::StreetObstacleParam &sop, lms::imaging::BGRAImageGraphics &debugGraphics);

};

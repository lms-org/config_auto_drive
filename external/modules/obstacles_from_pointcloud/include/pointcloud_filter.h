#pragma once

#include <lms/module.h>
#include <lms/math/polyline.h>
#include <street_environment/road.h>
#include <lms/math/point_cloud.h>

/**
 * @brief LMS module PointCloudFilter
 **/
class PointCloudFilter : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    lms::ReadDataChannel<bool> newData;
    lms::ReadDataChannel<lms::math::PointCloud2f> pointCloud;
    lms::WriteDataChannel<lms::math::PointCloud2f> sortedPointCloud;
};


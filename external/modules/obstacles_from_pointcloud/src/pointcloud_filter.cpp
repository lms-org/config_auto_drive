#include "pointcloud_filter.h"
#include <street_environment/obstacle.h>

bool PointCloudFilter::initialize() {
    newData = readChannel<bool>("NEW_DATA");
    pointCloud = readChannel<lms::math::PointCloud2f>("POINT_CLOUD_IN");
    sortedPointCloud = writeChannel<lms::math::PointCloud2f>("POINT_CLOUD_OUT");
    return true;
}

bool PointCloudFilter::deinitialize() {
    return true;
}

bool PointCloudFilter::cycle() {
    sortedPointCloud->points().clear();
    //check if the point is inside the car
    for(const lms::math::vertex2f &v:pointCloud->points()){
        if(v.x < 0.25 && v.x>-0.1 && v.y < 0.1 && v.y > -0.1){
            continue;
        }
        /*
        float minDistance = 100;
        for(lms::math::vertex2f rp:road->points()){
            float ndistance = rp.distance(v);
            if(ndistance < minDistance){
                minDistance = ndistance;
            }
        }
        if(minDistance>0.4){
            continue;
        }
        */
        sortedPointCloud->points().push_back(v);
    }
    return true;
}

#ifndef NEW_ROAD_DETECTION_H
#define NEW_ROAD_DETECTION_H

#include <lms/module.h>
#include <street_environment/road.h>


#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define USE_OPENCV
#include <lms/imaging/image.h>

/**
 * @brief LMS module new_road_detection
 **/
class NewRoadDetection : public lms::Module {

    struct SearchLine{
        lms::math::vertex2f w_start;
        lms::math::vertex2f w_end;
        lms::math::vertex2i i_start;
        lms::math::vertex2i i_end;
    };

public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
    void configsChanged() override;
    lms::ReadDataChannel<lms::imaging::Image> image;
    lms::ReadDataChannel<street_environment::RoadLane> road;
    lms::WriteDataChannel<lms::imaging::Image> debugImage;
    cv::Mat topView2cam;
    cv::Mat cam2topView;
    cv::Size topViewSize;
    cv::Mat topView;


    cv::Mat world2cam;
    cv::Mat cam2world;

    void test();
};

#endif // NEW_ROAD_DETECTION_H

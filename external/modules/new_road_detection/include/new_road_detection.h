#ifndef NEW_ROAD_DETECTION_H
#define NEW_ROAD_DETECTION_H

#include <lms/module.h>
#include <street_environment/road.h>
#include <street_environment/car.h>


#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define USE_OPENCV
#include <lms/imaging/image.h>

/**
 * @brief LMS module new_road_detection
 **/
class NewRoadDetection : public lms::Module {
    //cam trafo
    cv::Mat world2cam;
    cv::Mat cam2world;

    //Datachannels
    lms::ReadDataChannel<lms::imaging::Image> image;
    lms::WriteDataChannel<street_environment::RoadLane> road;
    lms::WriteDataChannel<street_environment::RoadLane> output;
    lms::WriteDataChannel<lms::imaging::Image> debugImage;
    lms::WriteDataChannel<lms::math::polyLine2f> debugAllPoints;
    lms::WriteDataChannel<lms::math::polyLine2f> debugValidPoints;
    lms::WriteDataChannel<lms::math::polyLine2f> debugTranslatedPoints;

    lms::ReadDataChannel<street_environment::Car> car;

    //for debugging
    cv::Mat topView2cam;
    cv::Mat cam2topView;
    cv::Size topViewSize;
    cv::Mat topView;

    struct SearchLine{
        lms::math::vertex2f w_start;
        lms::math::vertex2f w_end;
        lms::math::vertex2i i_start;
        lms::math::vertex2i i_end;


        lms::math::vertex2f w_left;
        lms::math::vertex2f w_mid;
        lms::math::vertex2f w_right;
    };

public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
    void configsChanged() override;
    bool find();
};

#endif // NEW_ROAD_DETECTION_H

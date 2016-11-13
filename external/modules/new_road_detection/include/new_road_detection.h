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

#include <list>

// for multithreading
#include <mutex>
#include <condition_variable>
#include <thread>

/**
 * @brief LMS module new_road_detection
 **/
class NewRoadDetection : public lms::Module {
    // configs
    float searchOffset;
    bool findPointsBySobel;
    bool renderDebugImage;
    float minLineWidthMul;
    float maxLineWidthMul;
    int threshold;
    float laneWidthOffsetInMeter;
    bool useWeights;
    int sobelThreshold;
    int numThreads; // 0 means single threaded

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

    std::list<SearchLine> lines;

    std::mutex mutex;
    std::vector<std::thread> threads;
    std::condition_variable conditionNewLine;
    std::condition_variable conditionLineProcessed;
    bool threadsRunning;
    int linesToProcess;
public:
    bool init() override;
    void destroy() override;
    bool cycle() override;
    void configsChanged() override;

    bool find();

    std::vector<lms::math::vertex2f>
    findBySobel(const bool renderDebugImage,
                const std::vector<int> &xv,
                const std::vector<int> &yv,
                const float minLineWidthMul,
                const float maxLineWidthMul,
                const float iDist,
                const float wDist,
                const int threshold);

    std::vector<lms::math::vertex2f>
    findByBrightness(const bool renderDebugImage,
                     const std::vector<int> &xv,
                     const std::vector<int> &yv,
                     const float minLineWidthMul,
                     const float maxLineWidthMul,
                     const float iDist,
                     const float wDist,
                     const int threshold);

    void processSearchLine(const SearchLine &line);

    void threadFunction();
};

#endif // NEW_ROAD_DETECTION_H

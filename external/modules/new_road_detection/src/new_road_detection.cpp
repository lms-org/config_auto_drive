#include "new_road_detection.h"
#include <lms/imaging/warp.h>
#include <lms/math/curve.h>
#include <lms/imaging/graphics.h>

bool NewRoadDetection::initialize() {
    image = readChannel<lms::imaging::Image>("IMAGE");
    road = readChannel<street_environment::RoadLane>("ROAD");
    debugImage = writeChannel<lms::imaging::Image>("DEBUG_IMAGE");


    configsChanged();
    return true;
}

bool NewRoadDetection::cycle() {
    if(image->width()*image->height() == 0){
        logger.error("cycle")<<"image size is 0";
        return false;
    }
    //TODO rectangle for neglecting areas

    //TODO calculate threshold for each line
    int threshold = config().get<int>("threshold",200);
    float minLineWidthMul = config().get<float>("minLineWidthMul",0.5);
    float maxLineWidthMul = config().get<float>("maxLineWidthMul",1.5);
    float searchOffset = config().get<float>("searchOffset",0.1);

    bool renderDebugImage = true;
    lms::imaging::BGRAImageGraphics graphics(*debugImage);
    if(renderDebugImage){
        //Clear debug image
        debugImage->resize(image->width(),image->height(),lms::imaging::Format::BGRA);
        debugImage->fill(0);
        lms::imaging::BGRAImageGraphics graphics(*debugImage);
        graphics.setColor(lms::imaging::red);
    }

    //create left/mid/right lane
    lms::math::polyLine2f mid = road->getWithDistanceBetweenPoints(config().get<float>("distanceBetweenSearchlines",0.2));
    lms::math::polyLine2f left = mid.moveOrthogonal(-0.4);
    lms::math::polyLine2f right = mid.moveOrthogonal(0.4);
    if(mid.points().size() != left.points().size() || mid.points().size() != right.points().size()){
        logger.error("invalid midlane given!");
        return false;
    }
    //get all lines
    std::vector<SearchLine> lines;
    std::vector<cv::Point2f> input;
    std::vector<cv::Point2f> output;
    for(int i = 0; i< (int)mid.points().size(); i++){
        SearchLine l;
        l.w_start = left.points()[i];
        l.w_end = right.points()[i];
        //check if the part is valid (middle should be always visible)
        if(!image->inside(l.w_start.x,l.w_start.y)){
            l.w_start = mid.points()[i];
            if(!image->inside(l.w_end.x,l.w_end.y)){
                continue;
            }
        }else if(!image->inside(l.w_end.x,l.w_end.y)){
            l.w_end = mid.points()[i];
        }

        input.emplace_back(l.w_start.x,l.w_start.y);
        input.emplace_back(l.w_end.x,l.w_end.y);
        lines.push_back(l);
    }
    //transform them in image-coordinates
    cv::perspectiveTransform(input,output,world2cam);
    //set image-coordinates
    int i = 0;
    //TODO keep them to reduce memory alloc.
    std::vector<int> xv;
    std::vector<int> yv;
    std::vector<std::uint8_t> color;
    for(SearchLine &l:lines){
        xv.clear();
        yv.clear();
        color.clear();
        l.i_start =lms::math::vertex2i((int)output[i].x,(int)output[i].y);
        i++;
        l.i_end = lms::math::vertex2i((int)output[i].x,(int)output[i].y);
        i++;
        //TODO get line
        //calculate the offset
        float iDist = l.i_start.distance(l.i_end);
        float wDist = l.w_start.distance(l.w_end);
        float pxlPerDist = iDist/wDist*searchOffset;
        lms::math::vertex2f iDiff = lms::math::vertex2f(l.i_start-l.i_end).normalize();
        lms::math::vertex2f startLine = lms::math::vertex2f(l.i_start)+iDiff*pxlPerDist;
        lms::math::vertex2f endLine = lms::math::vertex2f(l.i_end)-iDiff*pxlPerDist;
        //get all points in between
        lms::math::bresenhamLine(startLine.x,startLine.y,endLine.x,endLine.y,xv,yv);
        //get the color from the points
        if(renderDebugImage)
            graphics.setColor(lms::imaging::blue);
        for(int k = 0; k < (int)xv.size(); k++){
            const int x = xv[k];
            const int y = yv[k];
            if(!image->inside(x,y)){
                color.push_back(0);
                continue;
            }
            color.push_back(*(image->data()+x+y*image->width()));
            if(renderDebugImage)
                graphics.drawCross(x,y);
        }
        //detect peaks
        float pxlPeakWidth = iDist/wDist*0.02; //TODO to bad, calculate for each road line
        int tCounter = 0;
        if(renderDebugImage)
            graphics.setColor(lms::imaging::red);
        for(int k = 0; k < (int)color.size(); k++){
            if(color[k]>threshold){
                tCounter++;
            }else{
                if(tCounter > pxlPeakWidth*minLineWidthMul && tCounter < pxlPeakWidth*maxLineWidthMul){
                    if(renderDebugImage){
                        for(int j = 0; j<tCounter;j++){
                            graphics.drawCross(xv[k-j],yv[k-j]);
                        }
                    }
                    //TODO we found a valid point
                }
                tCounter = 0;
            }
        }
    }

    return true;
}

void NewRoadDetection::configsChanged(){

    //used to convert pxl from the top-down-view to the camera-view
    world2cam.create(3,3,CV_64F);
    std::vector<float> points = config().getArray<float>("cam2world");
    if(points.size() != 9){
        logger.error("invalid cam2world");
        return;
    }
    int i = 0;
    for(int r = 0; r < 3; r++) {
        for(int c = 0; c < 3; c++) {
            world2cam.at<double>(r, c) = points[i];
            i++;
        }
    }
    cam2world = world2cam.inv();


    //create a "viewport" which should be seen
    // Choose top-view image size
    topViewSize = cv::Size(512, 512);

    // Choose corner points (in real-world coordinates)
    std::vector<cv::Point2f> coordinates;
    coordinates.emplace_back(0, -1.500);
    coordinates.emplace_back(0, 1.500);
    coordinates.emplace_back(3.000, -1.500);
    coordinates.emplace_back(3.000, 1.500);

    //find homography
    std::vector<cv::Point2f> pixels;
    pixels.emplace_back(0, topViewSize.height);
    pixels.emplace_back(0, 0);
    pixels.emplace_back(topViewSize.width, topViewSize.height);
    pixels.emplace_back(topViewSize.width, 0);
    cv::Mat H = cv::findHomography(pixels, coordinates);

    //
    topView2cam = world2cam * H;
    cam2topView = topView2cam.inv();
}

bool NewRoadDetection::deinitialize() {
    return true;
}


void NewRoadDetection::test(){
    //from http://romsteady.blogspot.de/2015/07/calculate-opencv-warpperspective-map.html
    //DOES NOT WORK ATM

    /*
    cv::warpPerspective(image->convertToOpenCVMat(), topView, cam2topView, topViewSize,cv::INTER_NEAREST);
    cv::imshow("homography_estimator_preview", topView);
    cv::waitKey(1);
    std::vector<cv::Point2f> input;
    std::vector<cv::Point2f> output;
    input.emplace_back(0,0);
    cv::perspectiveTransform(input,output,world2cam);
    cv::perspectiveTransform(input,output,cam2world);
    logger.error("WASD")<<output.size()<< " "<<output[0].x << " "<<output[0].y;
    */
    //transform points http://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html?highlight=perspective#perspectivetransform
    //test();

    cv::Mat sourceImage = image->convertToOpenCVMat();
    cv::Mat destinationImage;

    std::vector<cv::Point2f> destinationCorners;
    destinationCorners.emplace_back(0, -1.500);
    destinationCorners.emplace_back(0, 1.500);
    destinationCorners.emplace_back(3.000, -1.500);
    destinationCorners.emplace_back(3.000, 1.500);

    //find homography
    std::vector<cv::Point2f> originalCorners;
    originalCorners.emplace_back(0, topViewSize.height);
    originalCorners.emplace_back(0, 0);
    originalCorners.emplace_back(topViewSize.width, topViewSize.height);
    originalCorners.emplace_back(topViewSize.width, 0);

    //from link
    cv::Mat transformationMatrix = cv::getPerspectiveTransform(destinationCorners,originalCorners);

    // Since the camera won't be moving, let's pregenerate the remap LUT
    cv::Mat inverseTransMatrix;
    cv::invert(transformationMatrix, inverseTransMatrix);

    // Generate the warp matrix
    cv::Mat map_x, map_y, srcTM;
    srcTM = inverseTransMatrix.clone(); // If WARP_INVERSE, set srcTM to transformationMatrix

    //TODO weiss nicht ob das stimmt
    cv::Mat sourceFrame = sourceImage;


    map_x.create(sourceFrame.size(), CV_32FC1);
    map_y.create(sourceFrame.size(), CV_32FC1);

    double M11, M12, M13, M21, M22, M23, M31, M32, M33;
    M11 = srcTM.at<double>(0,0);
    M12 = srcTM.at<double>(0,1);
    M13 = srcTM.at<double>(0,2);
    M21 = srcTM.at<double>(1,0);
    M22 = srcTM.at<double>(1,1);
    M23 = srcTM.at<double>(1,2);
    M31 = srcTM.at<double>(2,0);
    M32 = srcTM.at<double>(2,1);
    M33 = srcTM.at<double>(2,2);

    for (int y = 0; y < sourceFrame.rows; y++) {
      double fy = (double)y;
      for (int x = 0; x < sourceFrame.cols; x++) {
        double fx = (double)x;
        double w = ((M31 * fx) + (M32 * fy) + M33);
        w = w != 0.0f ? 1.f / w : 0.0f;
        float new_x = (float)((M11 * fx) + (M12 * fy) + M13) * w;
        float new_y = (float)((M21 * fx) + (M22 * fy) + M23) * w;
        map_x.at<float>(y,x) = new_x;
        map_y.at<float>(y,x) = new_y;
      }
    }
    cv::Mat transformation_x,transformation_y;
    // This creates a fixed-point representation of the mapping resulting in ~4% CPU savings
    transformation_x.create(sourceFrame.size(), CV_16SC2);
    transformation_y.create(sourceFrame.size(), CV_16UC1);
    cv::convertMaps(map_x, map_y, transformation_x, transformation_y, false);

    // If the fixed-point representation causes issues, replace it with this code
    //transformation_x = map_x.clone();
    //transformation_y = map_y.clone();

    //I then apply the map using a remap call:

    cv::remap(sourceImage, destinationImage, transformation_x, transformation_y, CV_INTER_LINEAR);
    cv::imshow("homography_estimator_preview", destinationImage);
    cv::waitKey(1);

}

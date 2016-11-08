#include "new_road_detection.h"
#include <lms/imaging/warp.h>
#include <lms/math/curve.h>
#include <lms/imaging/graphics.h>
#include <local_course/local_course.h>

bool NewRoadDetection::initialize() {
    image = readChannel<lms::imaging::Image>("IMAGE");
    road = readChannel<street_environment::RoadLane>("ROAD");
    output = writeChannel<street_environment::RoadLane>("ROAD_OUTPUT");
    debugImage = writeChannel<lms::imaging::Image>("DEBUG_IMAGE");
    debugAllPoints = writeChannel<lms::math::polyLine2f>("DEBUG_ALL_POINTS");
    debugValidPoints = writeChannel<lms::math::polyLine2f>("DEBUG_VALID_POINTS");


    configsChanged();
    return true;
}

bool NewRoadDetection::cycle() {
    if(image->width()*image->height() == 0){
        logger.error("cycle")<<"image size is 0";
        return false;
    }
    //TODO rectangle for neglecting areas

    //(TODO calculate threshold for each line)
    const int threshold = config().get<int>("threshold",200);
    const float minLineWidthMul = config().get<float>("minLineWidthMul",0.5);
    const float maxLineWidthMul = config().get<float>("maxLineWidthMul",1.5);
    const float searchOffset = config().get<float>("searchOffset",0.1);
    const float laneWidthOffsetInMeter = config().get<float>("laneWidthOffsetInMeter",0.1);

    bool renderDebugImage = config().get<bool>("renderDebugImage",false);
    lms::imaging::BGRAImageGraphics graphics(*debugImage);
    if(renderDebugImage){
        //Clear debug image
        debugImage->resize(image->width(),image->height(),lms::imaging::Format::BGRA);
        debugImage->fill(0);
        lms::imaging::BGRAImageGraphics graphics(*debugImage);
        graphics.setColor(lms::imaging::red);
        debugAllPoints->points().clear();
        debugValidPoints->points().clear();
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
        l.w_left = left.points()[i];
        l.w_mid = mid.points()[i];
        l.w_right = right.points()[i];
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
    //keep them to reduce memory alloc.
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
        float pxlPeakWidth = iDist/wDist*0.02; //TODO to bad, calculate for each road line (how should we use them for searching?
        int tCounter = 0;
        std::vector<lms::math::vertex2f> foundPoints;
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

                        //we found a valid point
                        //get the middle
                        cv::Point2f mid(xv[k-tCounter/2],yv[k-tCounter/2]);
                        //transform it in world coordinates
                        std::vector<cv::Point2f> in;
                        in.push_back(mid);
                        std::vector<cv::Point2f> out;
                        cv::perspectiveTransform(in,out,cam2world);
                        //create lms vector for easier use
                        lms::math::vertex2f wMid(out[0].x,out[0].y);
                        //store it
                        foundPoints.push_back(wMid);
                    }
                }
                tCounter = 0;
            }
        }

        if(renderDebugImage){
            for(lms::math::vertex2f &v:foundPoints)
                debugAllPoints->points().push_back(v);
        }
        //filter
        //filter points that are in poluted regions (for example the car itself)
        //TODO
        //remove points with bad distances
        if(foundPoints.size() > 2){
            std::vector<int> foundCounter;
            foundCounter.resize(foundPoints.size(),0);
            for(std::size_t fp = 0; fp < foundPoints.size(); fp++){
                const lms::math::vertex2f &s = foundPoints[fp];
                for(std::size_t test = fp+1; test <foundPoints.size(); test++){
                    const lms::math::vertex2f &toTest  = foundPoints[test];
                    float distance = toTest.distance(s);
                    if((distance > 0.4-laneWidthOffsetInMeter && distance < 0.4 + laneWidthOffsetInMeter)|| (distance > 0.8-laneWidthOffsetInMeter && distance < 0.8+laneWidthOffsetInMeter)){
                        foundCounter[test]++;
                        foundCounter[fp]++;
                    }
                }
            }
            //filter, all valid points should have the same counter and have the highest number
            int max = 0;
            for(int c:foundCounter){
                if(c > max){
                    max = c;
                }
            }
            std::vector<lms::math::vertex2f> validPoints;
            for(int i = 0; i < (int)foundPoints.size(); i++){
                if(foundCounter[i] >= max){
                    validPoints.push_back(foundPoints[i]);
                }
            }
            /*
            if(validPoints.size() > 4){
                validPoints.clear();
            }
            */
            foundPoints = validPoints;
        }
        if(renderDebugImage){
            for(lms::math::vertex2f &v:foundPoints)
                debugValidPoints->points().push_back(v);
        }

        //Handle found points
        lms::math::vertex2f diff;
        std::vector<float> distances;
        for(int i = 0; i < (int)foundPoints.size(); i++){
            lms::math::vertex2f &wMind = foundPoints[i];
            float distanceToLeft = wMind.distance(l.w_left);
            float distanceToRight= wMind.distance(l.w_right);
            float distanceToMid= wMind.distance(l.w_mid);
            diff = (l.w_left-l.w_right).normalize();
            if(distanceToLeft < distanceToMid && distanceToLeft < distanceToRight){
                //left point
                wMind-=-diff*0.4;
                distances.push_back(distanceToLeft);
            }else if(distanceToRight < distanceToMid ){
                wMind+=diff*0.4;
                distances.push_back(distanceToRight);
            }else{
                distances.push_back(distanceToMid);
            }
        }
        std::vector<float> weights;
        lms::ServiceHandle<local_course::LocalCourse> localCourse = getService<local_course::LocalCourse>("LOCAL_COURSE_SERVICE");
        if(localCourse.isValid()){
            localCourse->addPoints(foundPoints,weights);
        }else{
            logger.error("localCourse invalid!");
            return false;
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
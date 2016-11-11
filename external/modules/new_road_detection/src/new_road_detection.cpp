#include "new_road_detection.h"
#include <lms/imaging/warp.h>
#include <lms/math/curve.h>
#include <lms/imaging/graphics.h>
#include <local_course/local_course.h>
#include <street_environment/car.h>

bool NewRoadDetection::initialize() {
    image = readChannel<lms::imaging::Image>("IMAGE");
    road = writeChannel<street_environment::RoadLane>("ROAD");
    //output = writeChannel<street_environment::RoadLane>("ROAD_OUTPUT");
    debugImage = writeChannel<lms::imaging::Image>("DEBUG_IMAGE");
    debugAllPoints = writeChannel<lms::math::polyLine2f>("DEBUG_ALL_POINTS");
    debugValidPoints = writeChannel<lms::math::polyLine2f>("DEBUG_VALID_POINTS");
    debugTranslatedPoints = writeChannel<lms::math::polyLine2f>("DEBUG_VALID_TRANSLATED_POINTS");

    car = readChannel<street_environment::Car>("CAR"); //TODO create ego-estimation service

    configsChanged();
    return true;
}

bool NewRoadDetection::cycle() {
    if(image->width()*image->height() == 0){
        logger.error("cycle")<<"image size is 0";
        return false;
    }
    //if we have a round, try to find the line
    if(road->points().size() > 1){
        find(); //TODO use bool from find
    }

    float dx = 0;
    float dy = 0;
    float dPhi = 0;
    //update the course
    if(config().get<bool>("translateEnvironment",false)){
        logger.info("translation")<<car->deltaPhi();
        //localCourse->update(car->localDeltaPosition().x,car->localDeltaPosition().y,car->deltaPhi()); //TODO x and y translation produce bad results
        double maxYawRate = 0.03;
        dPhi = car->deltaPhi();
        //TODO is there still a fail?
        if (dPhi < -maxYawRate){
            logger.error("invalid yawrate:")<<dPhi;
            dPhi = -maxYawRate;
        }else if (dPhi > maxYawRate){
            logger.error("invalid yawrate:")<<dPhi;
            dPhi = maxYawRate;
        }
    }

    lms::ServiceHandle<local_course::LocalCourse> localCourse = getService<local_course::LocalCourse>("LOCAL_COURSE_SERVICE");
    if(localCourse.isValid()){
        logger.time("localCourse");
        localCourse->update(dx,dy,dPhi);
        *road = localCourse->getCourse();
        logger.timeEnd("localCourse");
        bool renderDebugImage = config().get<bool>("renderDebugImage",false);
        if(renderDebugImage){
                debugTranslatedPoints->points() = localCourse->getPointsAdded();
        }

    }else{
        logger.error("localCourse invalid!");
        return false;
    }
    return true;
}
std::vector<lms::math::vertex2f> NewRoadDetection::findBySobel(const bool renderDebugImage, const std::vector<int> &xv,const std::vector<int> &yv, const float minLineWidthMul, const float maxLineWidthMul,const float iDist,const float wDist, const int threshold){
    lms::imaging::BGRAImageGraphics graphics(*debugImage);
    std::vector<lms::math::vertex2f> foundPoints;
    bool foundLowHigh = false;
    int pxlCounter = 0;
    for(int k = 1; k < (int)xv.size()-1; k++){
        const int xs = xv[k-1];
        const int ys = yv[k-1];
        const int x = xv[k];
        const int y = yv[k];
        const int xS = xv[k+1];
        const int yS = yv[k+1];
        //check if point is inside the image
        if(!image->inside(xs,ys) || !image->inside(x,y) || !image->inside(xS,yS)){
            continue;
        }
        if(renderDebugImage){
            graphics.setColor(lms::imaging::blue);
            graphics.drawCross(x,y);
        }

        int colors = *(image->data()+xs+ys*image->width());
        int colorS = *(image->data()+xS+yS*image->width());
        int sobel = colorS-colors;
        //da wir von links nach rechts suchen ist positiver sobel ein dunkel-hell übergang
        if(sobel > threshold){
            if(!foundLowHigh){
                foundLowHigh = true;
                pxlCounter = 0;
            }
            if(renderDebugImage){
                graphics.setColor(lms::imaging::green);
                graphics.drawCross(x,y,3);
            }
        }else if(sobel < -threshold){ //hell-dunkel übergang
            if(renderDebugImage){
                graphics.setColor(lms::imaging::yellow);
                graphics.drawCross(x,y,3);
            }
            //check if we found a lowHigh + highLow border
            if(foundLowHigh){
                //check if the points have the right distance
                float pxlPeakWidth = iDist/wDist*0.02; //TODO to bad, calculate for each road line (how should we use them for searching?
                if(pxlCounter > pxlPeakWidth*minLineWidthMul && pxlCounter < pxlPeakWidth*maxLineWidthMul){
                    //we found a valid poit, mark it
                    if(renderDebugImage){
                        graphics.setColor(lms::imaging::red);
                        for(int j = 0; j<pxlCounter;j++){
                            graphics.drawCross(xv[k-j],yv[k-j]);
                        }
                    }
                    //we found a valid point
                    //get the middle
                    cv::Point2f mid(xv[k-pxlCounter/2],yv[k-pxlCounter/2]);
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
            if(renderDebugImage && pxlCounter > 0){
                graphics.setColor(lms::imaging::green);
                graphics.drawCross(x,y);
            }
            pxlCounter = 0;
            foundLowHigh = false;
            //if not, we dont have to do anything
        }
        if(foundLowHigh){
            pxlCounter++;
        }
    }
    return foundPoints;
}
std::vector<lms::math::vertex2f> NewRoadDetection::findByBrightness(const bool renderDebugImage, const std::vector<int> &xv,const std::vector<int> &yv, const float minLineWidthMul, const float maxLineWidthMul,const float iDist,const float wDist, const int threshold){
    lms::imaging::BGRAImageGraphics graphics(*debugImage);
    std::vector<lms::math::vertex2f> foundPoints;
    std::vector<int> color;
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
    if(renderDebugImage)
        graphics.setColor(lms::imaging::red);
    for(int k = 0; k < (int)color.size(); k++){
        if(color[k]>threshold){
            tCounter++;
        }else{
            if(tCounter - k != 0 && tCounter > pxlPeakWidth*minLineWidthMul && tCounter < pxlPeakWidth*maxLineWidthMul){
                if(renderDebugImage){
                    for(int j = 0; j<tCounter;j++){
                        graphics.drawCross(xv[k-j],yv[k-j]);
                    }
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
            tCounter = 0;
        }
    }
    return foundPoints;
}

bool NewRoadDetection::find(){

    //TODO rectangle for neglecting areas
    bool findPointsBySobel = config().get<bool>("findBySobel",true);

    //(TODO calculate threshold for each line)
    const int threshold = config().get<int>("threshold",200);
    const float minLineWidthMul = config().get<float>("minLineWidthMul",0.5);
    const float maxLineWidthMul = config().get<float>("maxLineWidthMul",1.5);
    const float searchOffset = config().get<float>("searchOffset",0.1);
    const float laneWidthOffsetInMeter = config().get<float>("laneWidthOffsetInMeter",0.1);
    const bool useWeights = config().get<bool>("useWeights",false);
    bool renderDebugImage = config().get<bool>("renderDebugImage",false);
    int sobelThreshold = config().get<int>("sobelThreshold",200);
    if(renderDebugImage){
        //Clear debug image
        debugImage->resize(image->width(),image->height(),lms::imaging::Format::BGRA);
        debugImage->fill(0);
        debugAllPoints->points().clear();
        debugValidPoints->points().clear();
        debugTranslatedPoints->points().clear();
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


    //TODO multithread the following part (each line could be handled independently)

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
        lms::math::bresenhamLine(startLine.x,startLine.y,endLine.x,endLine.y,xv,yv); //wir suchen von links nach rechts!

        //find points
        std::vector<lms::math::vertex2f> foundPoints;
        if(findPointsBySobel){
           foundPoints = findBySobel(renderDebugImage,xv,yv,minLineWidthMul,maxLineWidthMul,iDist,wDist,sobelThreshold);
        }else{
           foundPoints = findByBrightness(renderDebugImage,xv,yv,minLineWidthMul,maxLineWidthMul,iDist,wDist,threshold);
        }

        if(renderDebugImage){
            for(lms::math::vertex2f &v:foundPoints)
                debugAllPoints->points().push_back(v);
        }

        //filter
        //TODO filter points that are in poluted regions (for example the car itself)
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
            //TODO if we have more than 3 points we know that there is an error!
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
                wMind-=diff*0.4;
                distances.push_back(distanceToLeft);
            }else if(distanceToRight < distanceToMid ){
                wMind+=diff*0.4;
                distances.push_back(distanceToRight);
            }else{
                distances.push_back(distanceToMid);
            }
        }

        std::vector<float> weights;
        for(const float &dist:distances){
            //as distance is in meter, we multiply it by 100
            if(useWeights)
                weights.push_back(1/(dist*100+0.001)); //TODO hier etwas sinnvolles überlegen
            else
                weights.push_back(1);
        }
        /*
        if(renderDebugImage){
            for(lms::math::vertex2f &v:foundPoints)
                debugTranslatedPoints->points().push_back(v);
        }
        */
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

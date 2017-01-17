#include "crossing_detection.h"
#include "local_course/local_course.h"
#include "lms/imaging/warp.h"
#include "lms/imaging_detection/line_point.h"
#include "detection_utils.h"
#include "warp_service/warp_service.h"
#include "lms/imaging_detection/street_crossing.h"

bool CrossingDetection::init(){
    image = readChannel<lms::imaging::Image>("IMAGE");
    imageDebug = writeChannel<lms::imaging::Image>("DEBUG_IMAGE");
    //points = readChannel<lms::math::polyLine2f> ("POINTS");
    env = writeChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_CROSSING");
    configsChanged();
    return true;
}

void CrossingDetection::destroy(){
}

bool CrossingDetection::cycle(){
    if(renderDebugImage){
        imageDebug->resize(image->width(),image->height(),lms::imaging::Format::BGRA);
        imageDebug->fill(0);
    }
    env->objects.clear();
    find();
    return true;
}

void CrossingDetection::configsChanged(){
    renderDebugImage = config().get<bool>("renderDebugImage",false);
    threshold = config().get<int>("threshold",100);
    lms::ServiceHandle<warp_service::WarpService> warp = getService<warp_service::WarpService>("WARP_SERVICE");
    if(!warp.isValid()){
        logger.error("invalid warpservice")<<"I will fail just for you";
    }
    homo = warp->getHomography();

}

bool CrossingDetection::find(){
    lms::math::polyLine2f line;
    {
        //we just want a curse and don't mind if it's from the last cycle
        lms::ServiceHandle<local_course::LocalCourse> course = getService<local_course::LocalCourse>("LOCAL_COURSE_SERVICE");
        if(course.isValid()){
            line = course->getCourse().moveOrthogonal(0.2);
        }
    }
    if(line.points().size() == 0){
        logger.warn("No road given");
        return false;
    }
    bool foundCrossing = false;
    bool foundStartLine = false;
    lms::math::vertex2f middlePosition;
    lms::math::vertex2f viewDirection;
    //trying to find the stop-line
    std::vector<int> xv;
    std::vector<int> yv;
    for(std::size_t i = 1; i < line.points().size(); i++){
        const lms::math::vertex2f &bot = line.points()[i-1];
        lms::math::vertex2i iBot;
        const lms::math::vertex2f &top = line.points()[i];
        lms::math::vertex2i iTop;
        homo.vti(iBot,bot);
        homo.vti(iTop,top);
        if(!image->inside(iTop.x,iTop.y) || !image->inside(iBot.x,iBot.y)){
            continue;
        }
        float iDist = iBot.distance(iTop);
        float wDist = bot.distance(top);
        lms::math::bresenhamLine(iBot.x,iBot.y,iTop.x,iTop.y,xv,yv);
        std::vector<lms::math::vertex2f> points = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,0.2,2,0.04,iDist,wDist,threshold,homo);
        if(points.size() > 0){
            //trying to detect the stopline
            if(points.size() == 1){
                middlePosition = points[0];
                viewDirection = (top-bot).normalize();//TODO we could also use the found points
                //now we go one step to the left/right and check if we still can find a point
                float offsetSide = 0.1;
                float offsetAlong = 0.1;
                lms::math::vertex2f diff = top-bot;
                diff = diff.normalize();
                lms::math::vertex2f orth = diff.rotateClockwise90deg();
                lms::math::vertex2f right = middlePosition+orth*offsetSide-diff*offsetAlong;
                lms::math::vertex2f left = middlePosition-orth*offsetSide-diff*offsetAlong;
                getXYfromPoint(right,right+diff*2*offsetAlong,xv,yv,homo);
                std::vector<lms::math::vertex2f> pointsRight = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,0.2,2,0.04,iDist,wDist,threshold,homo);
                getXYfromPoint(left,left+diff*2*offsetAlong,xv,yv,homo);
                std::vector<lms::math::vertex2f> pointsLeft = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,0.2,2,0.04,iDist,wDist,threshold,homo);
                if(pointsRight.size() != 1 && pointsLeft.size() != 1){
                    //no crossing, something else
                    break;
                }
                //check if it a crossing, not a startline
                getXYfromPoint(middlePosition-orth*0.4-diff*offsetAlong,middlePosition-orth*0.4+diff*offsetAlong,xv,yv,homo);
                std::vector<lms::math::vertex2f> pointsStartline = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,0.2,2,0.04,iDist,wDist,threshold,homo);
                if(pointsStartline.size() != 0){
                    //we found a start line
                    foundStartLine = true;
                    break;
                }

                //trying to find oposite stop-line
                //check if it a crossing, not a startline
                getXYfromPoint(middlePosition-orth*0.4+diff*0.7,middlePosition-orth*0.4+diff*0.9,xv,yv,homo);
                std::vector<lms::math::vertex2f> pointsOppositeStopline = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,0.1,1,0.04,iDist,wDist,threshold,homo);
                if(pointsOppositeStopline.size() != 1){
                    break;
                }
                foundCrossing = true;
            }
            break;
        }
    }
    /*float orth;
    float tang;
    line.distance(middlePosition,orth,tang);
    middlePosition = line.interpolateAtDistance(tang);*/
    //Check if it blocked
    if(foundStartLine){
        street_environment::StartLinePtr startline(new street_environment::StartLine());
        startline->addPoint(middlePosition);
        startline->viewDirection(viewDirection);
        startline->width(0.2);
        startline->setTrust(1);
        env->objects.push_back(startline);
    }else if(foundCrossing){
        street_environment::CrossingPtr crossing(new street_environment::Crossing());
        crossing->addPoint(middlePosition);
        crossing->viewDirection(viewDirection);
        crossing->width(0.2);
        crossing->setTrust(1);
        env->objects.push_back(crossing);

    }
    return true;
}



/*
    gaussBuffer->resize(image->width(),image->height(), image->format());
    gaussBuffer->fill(0);

    lms::imaging::detection::StreetCrossing crossing;
    lms::imaging::detection::StreetCrossing::StreetCrossingParam scp;
    scp.target = image.get();
    scp.gaussBuffer = gaussBuffer;
    scp.fromConfig(&config("defaultEPParameter"));
    scp.fromConfig(&config("defaultLPParameter"));
    scp.fromConfig(&config("defaultLineParameter"));
    scp.lineWidthMax = scp.lineWidthMax*2;
    scp.boxDepthSearchLength = config("defaultCrossingParameter").get<float>("boxDepthSearchLength",20);
    scp.obstacleRightOffset = config("defaultCrossingParameter").get<float>("obstacleRightOffset",0.1);
    scp.obstacleSobelThreshold = config("defaultCrossingParameter").get<float>("obstacleSobelThreshold",100);
    scp.boxPointsNeeded = config("boxPointsNeeded").get<float>("boxPointsNeeded",3);
    scp.maxIterationsRANSAC = config("defaultCrossingParameter").get<int>("maxIterationsRANSAC",100);
    scp.inlierThresholdRANSAC = config("defaultCrossingParameter").get<double>("inlierThresholdRANSAC",4.0);
    scp.middleLine = line;
    /*
    for(int i = 0; i < (int)scp.middleLine.points().size();){
        if(scp.middleLine.points()[i].length() < 0.3){
            scp.middleLine.points().erase(scp.middleLine.points().begin()+i);
        }else{
            i++;
        }
    }

    lms::imaging::BGRAImageGraphics graphics(*imageDebug.get());
    crossing.find(scp,graphics);

    */

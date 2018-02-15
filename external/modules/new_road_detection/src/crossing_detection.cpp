#include "crossing_detection.h"
#include "local_course/local_course.h"
#include "lms/imaging/warp.h"
#include "lms/imaging_detection/line_point.h"
#include "detection_utils.h"
#include "warp_service/warp_service.h"
#include "lms/imaging_detection/street_crossing.h"
#include "phoenix_CC2016_service/phoenix_CC2016_service.h"

bool CrossingDetection::init(){
    image = readChannel<lms::imaging::Image>("IMAGE");
    imageDebug = writeChannel<lms::imaging::Image>("DEBUG_IMAGE");
    //points = readChannel<lms::math::polyLine2f> ("POINTS");
    env = writeChannel<street_environment::EnvironmentObjects>("ENVIRONMENT");
    configsChanged();
    started = false;
    return true;
}

void CrossingDetection::destroy(){
}

bool CrossingDetection::cycle(){
    if(renderDebugImage){
        imageDebug->resize(image->width(),image->height(),lms::imaging::Format::BGRA);
        imageDebug->fill(0);
    }


    if(phoenix_CC2016_service::CCDriveMode::IDLE != getService<phoenix_CC2016_service::Phoenix_CC2016Service>("PHOENIX_SERVICE")->driveMode())
    {
        if(!started)
        {
            startTime = lms::Time::now();
            started = true;
        }
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
            line = course->getCourse().getWithDistanceBetweenPoints(0.2).moveOrthogonal(0.2);
        }
    }
    if(line.points().size() == 0){
        logger.warn("No road given");
        return false;
    }
    //trying to find the stop-line
    std::vector<int> xv;
    std::vector<int> yv;
    const float minLineWidthMul = config().get<float>("minLineWidthMul",0.2);
    const float maxLineWidthMul = config().get<float>("maxLineWidthMul",2);
    const float offsetSide = config().get<float>("offsetSide",0.1);
    const float offsetAlong = config().get<float>("offsetAlong",0.1);
    const float lineWidth = config().get<float>("lineWidth",0.04);
    for(std::size_t i = 3; i < line.points().size(); i++){
        const lms::math::vertex2f &bot = line.points()[i-1];
        lms::math::vertex2i iBot;
        const lms::math::vertex2f &top = line.points()[i];
        lms::math::vertex2i iTop;
        homo.vti(iBot,bot);
        homo.vti(iTop,top);
        if(!image->inside(iTop.x,iTop.y) || !image->inside(iBot.x,iBot.y)){
            continue;
        }
        const float iDist = iBot.distance(iTop);
        const float wDist = bot.distance(top);
        lms::math::bresenhamLine(iBot.x,iBot.y,iTop.x,iTop.y,xv,yv);
        //std::cout<<"crossing START middle"<<std::endl;

        std::vector<lms::math::vertex2f> points = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,minLineWidthMul,maxLineWidthMul,lineWidth,iDist,wDist,threshold,homo);

        //std::cout<<"crossing END middle "<<points.size()<<std::endl;
        logger.debug("points") <<points.size();
        if(points.size() ==1){
            logger.debug("cycle") <<"found middle of right lane";
        //trying to detect the stopline
            const lms::math::vertex2f middlePosition = points[0];
            const lms::math::vertex2f viewDirection = (top-bot).normalize();
            //now we go one step to the left/right and check if we still can find a point
            const lms::math::vertex2f orth = viewDirection.rotateClockwise90deg();
            const lms::math::vertex2f right = middlePosition+orth*offsetSide-viewDirection*offsetAlong;
            const lms::math::vertex2f left = middlePosition-orth*offsetSide-viewDirection*offsetAlong;
            logger.debug("crossing search for left/right lane");
            getXYfromPoint(right,right+viewDirection*2*offsetAlong,xv,yv,homo);
            std::vector<lms::math::vertex2f> pointsRight = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,minLineWidthMul,maxLineWidthMul,lineWidth,iDist,wDist,threshold,homo);
            getXYfromPoint(left,left+viewDirection*2*offsetAlong,xv,yv,homo);
            std::vector<lms::math::vertex2f> pointsLeft = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,minLineWidthMul,maxLineWidthMul,lineWidth,iDist,wDist,threshold,homo);




            if(pointsRight.size() != 1 && pointsLeft.size() != 1){
                //no crossing, something else
                logger.info("did not find left or fight lane")<<(int)(pointsRight.size())<<" "<<(int)(pointsLeft.size());
                continue;
            }



            //check if it a crossing, not a startline
            logger.debug("crossing search for start lane");
            getXYfromPoint(middlePosition-orth*0.42-viewDirection*offsetAlong,middlePosition-orth*0.42+viewDirection*offsetAlong,xv,yv,homo);




            std::vector<lms::math::vertex2f> pointsStartline_left = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,minLineWidthMul,maxLineWidthMul,lineWidth,iDist,wDist,threshold,homo);
            logger.debug("pointsStartlineleft")<<pointsStartline_left.size();


            getXYfromPoint(middlePosition-orth*0.3-viewDirection*offsetAlong,middlePosition-orth*0.3+viewDirection*offsetAlong,xv,yv,homo);

            std::vector<lms::math::vertex2f> pointsStartline_right = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,minLineWidthMul,maxLineWidthMul,lineWidth,iDist,wDist,threshold,homo);
            logger.debug("pointsStartlineright")<<pointsStartline_right.size();



            if(pointsStartline_left.size() > 0 || pointsStartline_right.size() > 0){
                //we found a start line
                street_environment::StartLinePtr startline(new street_environment::StartLine());
                startline->addSensor("CAMERA");
                startline->addPoint(middlePosition);
                startline->viewDirection(viewDirection);
                startline->width(0.2);
                startline->setTrust(1);
                env->objects.push_back(startline);



                logger.info("found startline. Set mode to parking") << middlePosition.x << " " << middlePosition.y;

                double dist = sqrt(middlePosition.x * middlePosition.x + middlePosition.y *middlePosition.y );

                if(dist < config().get<float>("maxDistToStartline",0.7) && phoenix_CC2016_service::CCDriveMode::FOH == getService<phoenix_CC2016_service::Phoenix_CC2016Service>("PHOENIX_SERVICE")->driveMode())
                {
                    getService<phoenix_CC2016_service::Phoenix_CC2016Service>("PHOENIX_SERVICE")->setDriveMode(phoenix_CC2016_service::CCDriveMode::PARKING_START);
                }


                break;
            }else if(pointsStartline_left.size() > 1){
                logger.info("invalid startline, to many points on the left");
                continue;
            }

            //trying to find oposite stop-line
            //check if it a crossing, not a startline
            //starting at the middle of the left lane
            const float oppositeLineStart=config().get<float>("oppositeLineStart",0.7);
            const float oppositeLineEnd=config().get<float>("oppositeLineEnd",0.9);
            getXYfromPoint(middlePosition-orth*0.4+viewDirection*oppositeLineStart,middlePosition-orth*0.4+viewDirection*oppositeLineEnd,xv,yv,homo);
            std::vector<lms::math::vertex2f> pointsOppositeStopline = findBySobel(image.get(),imageDebug.get(),renderDebugImage,xv,yv,0.01,10,0.02,iDist,wDist,threshold,homo);


            if(pointsOppositeStopline.size() == 1){

                if(startTime.since() > lms::Time::fromMillis(2000))
                {
                    street_environment::CrossingPtr crossing(new street_environment::Crossing());
                    crossing->addPoint(middlePosition);
                    crossing->viewDirection(viewDirection);
                    crossing->width(0.2);
                    crossing->setTrust(1);
                    crossing->addSensor("CAMERA");
                    env->objects.push_back(crossing);
                    logger.debug("found crossing");
                }
                break;
            }else{
                logger.info("could not find opposite line! Found points: ") << pointsOppositeStopline.size();
            }
        }
    }
    env.publish();
    return true;
}


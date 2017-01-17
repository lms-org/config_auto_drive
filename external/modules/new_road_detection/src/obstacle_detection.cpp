#include "obstacle_detection.h"
#include "local_course/local_course.h"
#include "lms/imaging/warp.h"
#include "lms/imaging_detection/line_point.h"
#include "detection_utils.h"
#include "warp_service/warp_service.h"

bool ObstacleDetection::init(){
    gaussBuffer = new lms::imaging::Image();
    imageDebug = writeChannel<lms::imaging::Image>("DEBUG_IMAGE");
    image = readChannel<lms::imaging::Image>("IMAGE");
    env = writeChannel<street_environment::EnvironmentObjects>("ENVIRONMENT_OBSTACLE");
    configsChanged();
    return true;
}

void ObstacleDetection::destroy(){
}

bool ObstacleDetection::cycle(){
    if(renderDebugImage){
        imageDebug->resize(image->width(),image->height(),lms::imaging::Format::BGRA);
        imageDebug->fill(0);
    }
    //set the gaussbuffer
    gaussBuffer->resize(image->width(),image->height(),lms::imaging::Format::GREY);

    env->objects.clear();
    const lms::math::polyLine2f middle;

    lms::imaging::detection::StreetObstacle::StreetObstacleParam sopRight;
    lms::imaging::detection::StreetObstacle::StreetObstacleParam sopLeft;
    sopRight.obstacleRight = true;
    sopRight.edge = true;
    sopRight.target = image.get();
    sopRight.gaussBuffer = gaussBuffer;
    sopRight.fromConfig(&config("defaultEPParameter"));
    sopRight.fromConfig(&config("defaultLPParameter"));
    sopRight.fromConfig(&config("defaultLineParameter"));
    sopRight.fromConfig(&config("defaultObstacleParameter"));
    for(const lms::math::vertex2f &v:middle.points()){
        if(v.length() <= 0.4)
            continue;
        sopRight.middleLine.points().push_back(v);
    }
    lms::imaging::BGRAImageGraphics debugGraphics(*imageDebug);
    if(config().get<bool>("searchObstacleLeft",true)){
        sopLeft.obstacleRight = true;
        find(sopLeft,debugGraphics);
    }

    if(config().get<bool>("searchObstacleRight",true)){
        sopLeft = sopRight;
        sopLeft.obstacleRight = false;
        find(sopLeft,debugGraphics);
    }

    return true;
}

void ObstacleDetection::configsChanged(){
    renderDebugImage = config().get<bool>("renderDebugImage",false);
    threshold = config().get<int>("threshold",100);
    lms::ServiceHandle<warp_service::WarpService> warp = getService<warp_service::WarpService>("WARP_SERVICE");
    if(!warp.isValid()){
        logger.error("invalid warpservice")<<"I will fail just for you";
    }
    homo = warp->getHomography();

}

void ObstacleDetection::find(const lms::imaging::detection::StreetObstacle::StreetObstacleParam &sop,lms::imaging::BGRAImageGraphics &debugGraphics){
    lms::imaging::detection::StreetObstacle imageObstacle;
    if(imageObstacle.find(sop,debugGraphics)){
        std::shared_ptr<street_environment::Obstacle> obstacleW(new street_environment::Obstacle());
        logger.debug(getName())<<"found obstacle pointCount: "<<&imageObstacle.results<<" "<<imageObstacle.results.size();
        for(const lms::imaging::detection::Line &line:imageObstacle.results){
            logger.debug(getName())<<"found obstacle pointCount: "<<line.points().size();
            //get the position
            lms::math::vertex2f pos(0,0);
            lms::math::vertex2f tmp;
            for(const lms::imaging::detection::LinePoint &lp:line.points()){
                lms::math::vertex2i v(lp.low_high.x,lp.low_high.y);
                lms::imaging::C2V(&v,&tmp);
                pos += tmp;
            }
            pos /= (float)line.points().size();
            logger.debug("cycle")<<"adding obstacle at"<<pos.x << " "<<pos.y;
            //get the viewDirection

            lms::math::vertex2i i1(line.points()[0].low_high.x,line.points()[0].low_high.y);
            lms::math::vertex2i i2(line.points()[line.points().size()-1].low_high.x,line.points()[line.points().size()-1].low_high.y);

            lms::math::vertex2f pos1(0,0);
            lms::math::vertex2f pos2(0,0);
            lms::imaging::C2V(&i1,&pos1);
            lms::imaging::C2V(&i2,&pos2);
            lms::math::vertex2f viewDir = (pos1-pos2).rotateAntiClockwise90deg();
            float obstWidth = pos1.distance(pos2);

            obstacleW->setTrust(0.1);//Set default trust
            obstacleW->addPoint(pos);
            obstacleW->name("right_obst");

            //TODO check if obstacle should be added
            //TODO check if the line is perpendicular to the road
            obstacleW->width(obstWidth);
            env->objects.push_back(obstacleW);
        }
    }
}

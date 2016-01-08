#include "image_object_renderer.h"
#include "lms/imaging_detection/line.h"
#include "street_environment/start_line.h"

#include "lms/imaging/warp.h"
#include "area_of_detection/area_of_detection.h"

bool ImageObjectRenderer::initialize() {
    //get all elements that you want to draw
    int imageWidth = config().get<int>("imageWidth",512);
    int imageHeight = config().get<int>("imageHeight",512);

    drawObjectStrings = config().getArray<std::string>("envObjects");

    //create the image you want to draw on
    image = writeChannel<lms::imaging::Image>("IMAGE");
    image->resize(imageWidth,imageHeight,lms::imaging::Format::BGRA);
    graphics = new lms::imaging::BGRAImageGraphics(*image);

    for(std::string &obj: drawObjectStrings){
        drawObjects.push_back(readChannel<lms::Any>(obj));
    }
    return true;
}

float ImageObjectRenderer::translateY(float y){
    return -y*image->width()/5+image->width()/2;
}

float ImageObjectRenderer::translateX(float x){
    return x*image->height()/5;
}

bool ImageObjectRenderer::deinitialize() {
    return true;
}

bool ImageObjectRenderer::cycle() {
   image->fill(0);
    for(lms::ReadDataChannel<lms::Any> &dO :drawObjects){
        //set the color
        logger.debug("trying to draw: ")<< dO.name();
        bool customColor = setColor(dO.name());
        if(dO.castableTo<lms::math::polyLine2f>()){//lms::math::polyLine2f
            drawPolyLine(dO.getWithType<lms::math::polyLine2f>());
        }else if(dO.castableTo<lms::math::vertex2f>()){
            drawVertex2f(*dO.getWithType<lms::math::vertex2f>());
            logger.debug("")<< "drawing v2f";
        }else if(dO.castableTo<street_environment::EnvironmentObjects>()){
            logger.debug("")<< "drawing evo";
            for(const std::shared_ptr<street_environment::EnvironmentObject> &eo:
                    (dO.getWithType<street_environment::EnvironmentObjects>()->objects)){
                    drawObject(eo.get(), customColor);
            }
        }else if(dO.castableTo<street_environment::TrajectoryPoint>()){
            logger.debug("")<< "drawing 4f";
            drawTrajectoryPoint(*(dO.getWithType<street_environment::TrajectoryPoint>()));
        }else if(dO.castableTo<std::vector<lms::math::Rect>>()){
            logger.debug("")<< "drawing rects";
            for(lms::math::Rect r: *dO.getWithType<std::vector<lms::math::Rect>>()){
                drawRect(r);
            }
        }else{
            logger.warn("cycle")<<"No valid type for "<<dO.name();
        }
    }
    return true;
}

void ImageObjectRenderer::drawRect(lms::math::Rect &r){
    drawLine(r.x,r.y,r.x,r.y+r.height);
    drawLine(r.x+r.width,r.y,r.x+r.width,r.y+r.height);
    drawLine(r.x,r.y+r.height,r.x+r.width,r.y+r.height);
    drawLine(r.x,r.y,r.x+r.width,r.y);
}


void ImageObjectRenderer::drawLine(float x1, float y1, float x2, float y2){

    int y1_ = -y1*image->width()/5+image->width()/2;
    int x1_ = x1*image->height()/5;
    int y2_ = -y2*image->width()/5+image->width()/2;
    int x2_ = x2*image->height()/5;
    graphics->drawLine(x1_,y1_,x2_,y2_);
}

void ImageObjectRenderer::drawVertex2f(const lms::math::vertex2f &v){
    int y = -v.y*image->width()/5+image->width()/2;
    int x = v.x*image->height()/5;
    graphics->drawCross(x,y,5);
}

void ImageObjectRenderer::drawTrajectoryPoint(const street_environment::TrajectoryPoint &v){
    drawVertex2f(lms::math::vertex2f(v.position.x,v.position.y));
    //TODO draw direction
}

void ImageObjectRenderer::drawObject(const street_environment::EnvironmentObject *eo, bool customColor){
    if(eo->getType() == 0){
        const street_environment::RoadLane &lane = eo->getAsReference<const street_environment::RoadLane>();
        drawPolyLine(&lane);
    }else if(eo->getType() == 1){
        const street_environment::Obstacle &obst = eo->getAsReference<const street_environment::Obstacle>();
        if(!customColor){
            if(obst.trust() > config().get<int>("obstacleTrustThreshold",0)){
                setColor("OBSTACLE_DETECTED");
            }else{
                setColor("DEFAULT_OBSTACLE");
            }
        }
        drawObstacle(&obst);
    }else if(eo->getType() == 2){
        setColor("CROSSING");
        const street_environment::Crossing &crossing = eo->getAsReference<const street_environment::Crossing>();
        drawObstacle(&crossing);
    }else if(eo->getType() == 3){
        const street_environment::StartLine &start = eo->getAsReference<const street_environment::StartLine>();
        setColor("START_LINE");
        drawObstacle(&start);
    }
}

void ImageObjectRenderer::drawObstacle(const street_environment::Obstacle *obstacle){
    drawVertex2f(obstacle->position());
}

void ImageObjectRenderer::drawPolyLine(const lms::math::polyLine2f *lane){
    lms::imaging::BGRAImageGraphics g(*image);
    int xOld = 0;
    int yOld = 0;
    logger.debug("drawPolyLine")<<"points: "<<lane->points().size();
    for(uint i = 0; i < lane->points().size(); i++){
        lms::math::vertex2f v = lane->points()[i];
        //TODO add some translate method to the config...
        int y = -v.y*image->width()/5+image->width()/2;
        int x = v.x*image->height()/5;
        graphics->drawCross(x,y,5);
        if(i != 0){
            graphics->drawLine(x,y,xOld,yOld);
        }
        xOld = x;
        yOld = y;
    }
}

bool ImageObjectRenderer::setColor(std::string toDrawName){
    const lms::ModuleConfig* m_config = nullptr;
    bool customColor = false;
    if(hasConfig(toDrawName)){
        m_config = &config(toDrawName);
        customColor = true;
      }else{
        m_config = &config();
    }
    lms::imaging::ARGBColor color=lms::imaging::ARGBColor(m_config->get<int>("colorR"),
                                                           m_config->get<int>("colorG"),m_config->get<int>("colorB"));
    graphics->setColor(color);
    return customColor;
}

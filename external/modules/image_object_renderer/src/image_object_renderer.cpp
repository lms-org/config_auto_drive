#include "image_object_renderer.h"
#include "lms/imaging_detection/line.h"

#include "lms/imaging/warp.h"

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
        }else if(dO.castableTo<std::pair<lms::math::vertex2f,lms::math::vertex2f>>()){
            logger.debug("")<< "drawing 4f";
            drawVertex4f(*(dO.getWithType<std::pair<lms::math::vertex2f,lms::math::vertex2f>>()));
        }else{
            logger.warn("cycle")<<"No valid type for "<<dO.name();
        }
    }
    return true;
}

void ImageObjectRenderer::drawVertex2f(const lms::math::vertex2f &v){
    int y = -v.y*image->width()/5+image->width()/2;
    int x = v.x*image->height()/5;
    graphics->drawCross(x,y,5);
}

void ImageObjectRenderer::drawVertex4f(const std::pair<lms::math::vertex2f,lms::math::vertex2f> &v){
    drawVertex2f(lms::math::vertex2f(v.first.x,v.first.y));
}

void ImageObjectRenderer::drawObject(const street_environment::EnvironmentObject *eo, bool customColor){
    //TODO not sure if this works
    if(eo->getType() == 0){
        const street_environment::RoadLane &lane = eo->getAsReference<const street_environment::RoadLane>();
        drawPolyLine(&lane);
    }else if(eo->getType() == 1){
        if(!customColor){
            setColor("OBSTACLE");
        }
        const street_environment::Obstacle &obst = eo->getAsReference<const street_environment::Obstacle>();
        drawObstacle(&obst);
    }else if(eo->getType() == 2){
        const street_environment::Crossing &crossing = eo->getAsReference<const street_environment::Crossing>();
        drawObstacle(&crossing);
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
    if(!hasConfig(toDrawName)){
        m_config = &config();
        customColor = true;
      }else{
        m_config = &config(toDrawName);
    }
    lms::imaging::ARGBColor color=lms::imaging::ARGBColor(m_config->get<int>("colorR"),
                                                           m_config->get<int>("colorG"),m_config->get<int>("colorB"));
    graphics->setColor(color);
    return customColor;
}

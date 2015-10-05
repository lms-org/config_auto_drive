#include "image_object_renderer.h"
#include "lms/imaging_detection/line.h"
#include "lms/imaging/warp.h"

bool ImageObjectRenderer::initialize() {
    //get all elements that you want to draw
    int imageWidth = getConfig()->get<int>("imageWidth",512);
    int imageHeight = getConfig()->get<int>("imageHeight",512);
    environments = getConfig()->getArray<std::string>("environments");
    polylines = getConfig()->getArray<std::string>("polylines");
    vertex2f = getConfig()->getArray<std::string>("vertex2f");
    vertex4f = getConfig()->getArray<std::string>("vertex4f");
    drawObjects = getConfig()->getArray<std::string>("envObjects");
    //create the image you want to draw on
    image = datamanager()->writeChannel<lms::imaging::Image>(this,"IMAGE");
    image->resize(imageWidth,imageHeight,lms::imaging::Format::BGRA);
    graphics = new lms::imaging::BGRAImageGraphics(*image);

    for(std::string obj: drawObjects){
        datamanager()->getReadAccess(this,obj);
    }
    for(std::string ev : environments){
        toDrawEnv.push_back(datamanager()->readChannel<street_environment::EnvironmentObjects>(this,ev));
    }
    for(std::string pl : polylines){
        toDrawPolyLines.push_back(datamanager()->readChannel<lms::math::polyLine2f>(this,pl));
    }
    for(std::string v : vertex2f){
        toDrawVertex2f.push_back(datamanager()->readChannel<lms::math::vertex2f>(this,v));
    }
    for(std::string v : vertex4f){
        toDrawVertex4f.push_back(datamanager()->readChannel<std::pair<lms::math::vertex2f,lms::math::vertex2f>>(this,v));
    }
    return true;
}

bool ImageObjectRenderer::deinitialize() {
    return true;
}

bool ImageObjectRenderer::cycle() {
   image->fill(0);
   for(uint i = 0; i < toDrawEnv.size(); i++){
        setColor(environments[i]);
        if(toDrawEnv[i]->objects.size() == 0){
            logger.info("cycle") <<"environment "<<environments[i] << " has no elements";
        }
        for(const std::shared_ptr<const street_environment::EnvironmentObject> &obj : toDrawEnv[i]->objects){
            drawObject(obj.get());
        }
    }

    for(uint i = 0; i < toDrawPolyLines.size(); i++){
        setColor(polylines[i]);
        drawPolyLine(toDrawPolyLines[i]);
    }

    for(uint i = 0; i < toDrawVertex2f.size(); i++){
        setColor(vertex2f[i]);
        drawVertex2f(*toDrawVertex2f[i]);
    }

    for(uint i = 0; i < toDrawVertex4f.size(); i++){
        setColor(vertex4f[i]);
        drawVertex4f(*toDrawVertex4f[i]);
    }

    for(std::string obj: drawObjects){
        setColor(obj);
        street_environment::EnvironmentObject *envO = datamanager()->getChannel<street_environment::EnvironmentObject>(obj,true);
        if(envO == nullptr){
            logger.warn("cycle")<<"invalid environmentObject "<< obj;
            continue;
        }
        drawObject(envO);
    }
    logger.timeEnd("TOTAL");
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

void ImageObjectRenderer::drawObject(const street_environment::EnvironmentObject *eo){
    //TODO not sure if this works
    if(eo->getType() == 0){
        const street_environment::RoadLane &lane = eo->getAsReference<const street_environment::RoadLane>();
        drawPolyLine(&lane);
    }else if(eo->getType() == 1){
        const street_environment::Obstacle &obst = eo->getAsReference<const street_environment::Obstacle>();
        drawObstacle(&obst);
    }
}

void ImageObjectRenderer::drawObstacle(const street_environment::Obstacle *obstacle){
    setColor("OBSTACLE");
    drawVertex2f(obstacle->position());
}

void ImageObjectRenderer::drawPolyLine(const lms::math::polyLine2f *lane){
    lms::imaging::BGRAImageGraphics g(*image);
    int xOld = 0;
    int yOld = 0;
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

void ImageObjectRenderer::setColor(std::string toDrawName){
    const lms::ModuleConfig* config = nullptr;
    if(!hasConfig(toDrawName)){
        config = getConfig();
      }else{
        config = getConfig(toDrawName);
    }
    lms::imaging::ARGBColor color=lms::imaging::ARGBColor(config->get<int>("colorR"),
                                                           config->get<int>("colorG"),config->get<int>("colorB"));
    graphics->setColor(color);
}

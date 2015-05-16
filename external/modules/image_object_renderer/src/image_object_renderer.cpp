#include "image_object_renderer.h"
#include "lms/imaging_detection/line.h"
#include "lms/imaging/warp.h"

bool ImageObjectRenderer::initialize() {
    //get all elements that you want to draw
    environments = getConfig()->getArray<std::string>("environments");
    polylines = getConfig()->getArray<std::string>("polylines");
    vertex2f = getConfig()->getArray<std::string>("vertex2f");
    vertex4f = getConfig()->getArray<std::string>("vertex4f");
    //create the image you want to draw on
    image = datamanager()->writeChannel<lms::imaging::Image>(this,"IMAGE");
    image->resize(512,512,lms::imaging::Format::BGRA);
    graphics = new lms::imaging::BGRAImageGraphics(*image);
    for(std::string ev : environments){
        toDrawEnv.push_back(datamanager()->readChannel<Environment>(this,ev));
    }
    for(std::string pl : polylines){
        toDrawPolyLines.push_back(datamanager()->readChannel<lms::math::polyLine2f>(this,pl));
    }
    for(std::string v : vertex2f){
        toDrawVertex2f.push_back(datamanager()->readChannel<lms::math::vertex2f>(this,v));
    }
    for(std::string v : vertex4f){
        toDrawVertex4f.push_back(datamanager()->readChannel<lms::math::vertex<4,float>>(this,v));
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
        for(const Environment::RoadLane &lane : toDrawEnv[i]->lanes){
            drawPolyLine(lane);
        }
    }
    for(uint i = 0; i < toDrawPolyLines.size(); i++){
        setColor(polylines[i]);
        drawPolyLine(*toDrawPolyLines[i]);
    }
    for(uint i = 0; i < toDrawVertex2f.size(); i++){
        setColor(vertex2f[i]);
        drawVertex2f(*toDrawVertex2f[i]);
    }
    for(uint i = 0; i < toDrawVertex4f.size(); i++){
        setColor(vertex4f[i]);
        drawVertex4f(*toDrawVertex4f[i]);
    }
    return true;
}

void ImageObjectRenderer::drawVertex2f(const lms::math::vertex2f &v){
    int y = -v.y()*image->width()/5+image->width()/2;
    int x = v.x()*image->height()/5;
    graphics->drawCross(x,y,5);
}

void ImageObjectRenderer::drawVertex4f(const lms::math::vertex<4,float> &v){
    drawVertex2f(lms::math::vertex2f(v[0],v[1]));
}

void ImageObjectRenderer::drawPolyLine(const lms::math::polyLine2f &lane){
    lms::imaging::BGRAImageGraphics g(*image);
    int xOld = 0;
    int yOld = 0;
    for(uint i = 0; i < lane.points().size(); i++){
        lms::math::vertex2f v = lane.points()[i];
        //TODO add some translate method to the config...
        int y = -v.y()*image->width()/5+image->width()/2;
        int x = v.x()*image->height()/5;
        graphics->drawCross(x,y,5);
        if(i != 0){
            graphics->drawLine(x,y,xOld,yOld);
        }
        xOld = x;
        yOld = y;
    }
}

void ImageObjectRenderer::setColor(std::string toDrawName){
    const lms::type::ModuleConfig* config = nullptr;
    if(!hasConfig(toDrawName)){
        config = getConfig();
      }else{
        config = getConfig(toDrawName);
    }
    lms::imaging::ARGBColor color=lms::imaging::ARGBColor(config->get<int>("colorR"),
                                                           config->get<int>("colorG"),config->get<int>("colorB"));
    graphics->setColor(color);
}

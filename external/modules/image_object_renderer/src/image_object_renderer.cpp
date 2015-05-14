#include "image_object_renderer.h"
#include "lms/imaging/find/line.h"
#include "lms/imaging/warp.h"

bool ImageObjectRenderer::initialize() {
    //TODO how do we set the color for each element?
    std::vector<std::string> environments = getConfig()->getArray<std::string>("environments");
    std::vector<std::string> polylines = getConfig()->getArray<std::string>("polylines");
    std::vector<std::string> vertex2f = getConfig()->getArray<std::string>("vertex2f");
    std::vector<std::string> vertex4f = getConfig()->getArray<std::string>("vertex4f");
    image = datamanager()->writeChannel<lms::imaging::Image>(this,"IMAGE");
    image->resize(512,512,lms::imaging::Format::BGRA);
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
    for(const Environment *evs:toDrawEnv){
        for(const Environment::RoadLane &lane : evs->lanes){
            drawPolyLine(lane);
        }
    }
    for(const lms::math::polyLine2f *pl:toDrawPolyLines){
        drawPolyLine(*pl);
    }
    for(const lms::math::vertex2f *v:toDrawVertex2f){
        drawVertex2f(*v);
    }
    for(const lms::math::vertex<4,float> *v:toDrawVertex4f){
        drawVertex4f(*v);
    }
    return true;
}

void ImageObjectRenderer::drawVertex2f(const lms::math::vertex2f &v){
    lms::imaging::BGRAImageGraphics g(*image);
    lms::imaging::ARGBColor color=lms::imaging::ARGBColor(getConfig()->get<int>("lColorR"),getConfig()->get<int>("lColorG"),getConfig()->get<int>("lColorB"));
    g.setColor(color);
    int y = -v.y()*image->width()/5+image->width()/2;
    int x = v.x()*image->height()/5;
    g.drawCross(x,y,5);
}

void ImageObjectRenderer::drawVertex4f(const lms::math::vertex<4,float> &v){
    drawVertex2f(lms::math::vertex2f(v[0],v[1]));
}

void ImageObjectRenderer::drawPolyLine(const lms::math::polyLine2f &lane){
    lms::imaging::BGRAImageGraphics g(*image);
    int xOld = 0;
    int yOld = 0;
    lms::imaging::ARGBColor color=lms::imaging::ARGBColor(getConfig()->get<int>("lColorR"),getConfig()->get<int>("lColorG"),getConfig()->get<int>("lColorB"));
    for(uint i = 0; i < lane.points().size(); i++){
        lms::math::vertex2f v = lane.points()[i];
        g.setColor(color);
        //TODO add some translate method to the config...
        int y = -v.y()*image->width()/5+image->width()/2;
        int x = v.x()*image->height()/5;
        g.drawCross(x,y,5);
        if(i != 0){
            g.drawLine(x,y,xOld,yOld);
        }
        xOld = x;
        yOld = y;
    }
}

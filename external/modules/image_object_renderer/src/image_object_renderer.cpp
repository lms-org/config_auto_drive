#include "image_object_renderer.h"
#include "lms/imaging/find/line.h"
#include "lms/imaging/warp.h"

bool ImageObjectRenderer::initialize() {
    //TODO
    image = datamanager()->writeChannel<lms::imaging::Image>(this,"IMAGE");
    image->resize(512,512,lms::imaging::Format::BGRA);
    toDraw.push_back(datamanager()->readChannel<Environment>(this,"ENVIRONMENT"));
    return true;
}

bool ImageObjectRenderer::deinitialize() {
    return true;
}

bool ImageObjectRenderer::cycle() {
    image->fill(0);
    //TODO just for testing, later on there has to be some config that states what ImageObjects of what should be drawn
    //set the name and the type
    for(const Environment *evs:toDraw){
        for(const Environment::RoadLane &lane : evs->lanes){
            drawRoadLane(lane);
        }
    }
    return true;
}

void ImageObjectRenderer::drawRoadLane(const Environment::RoadLane &lane){
    lms::imaging::BGRAImageGraphics g(*image);
    int xOld = 0;
    int yOld = 0;
    //logger.error() << lane.points.size();
    for(uint i = 0; i < lane.points().size(); i++){
        lms::math::vertex2f v = lane.points()[i];
        g.setColor(lms::imaging::ARGBColor(255,0,0));
        int y = -v.y()*image->width()/5+image->width()/2;
        int x = v.x()*image->height()/5;
        g.drawCross(x,y,5);

        if(i != 0){
            g.setColor(lms::imaging::ARGBColor(255,255,0));
            g.drawLine(x,y,xOld,yOld);
        }
        xOld = x;
        yOld = y;
    }
}

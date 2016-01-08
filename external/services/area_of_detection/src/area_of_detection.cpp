#include "area_of_detection/area_of_detection.h"


namespace area_of_detection {

bool AreaOfDetection::init() {
    configsChanged();
    return true;
}

void AreaOfDetection::configsChanged(){
    m_visibleAreas.clear();
    if(config().hasKey("visibleAreas")){
        std::vector<std::string> sRects = config().getArray<std::string>("visibleAreas");
        for(std::string &sRect: sRects){
            lms::math::Rect r;
            if(!config().hasKey(sRect+"_x") || !config().hasKey(sRect+"_y") ||!config().hasKey(sRect+"_w")||!config().hasKey(sRect+"_h")){
                logger.error("configsChanged")<<"visibleArea not valid: "<< sRect;
                return;
            }
            r.x = config().get<float>(sRect+"_x");
            r.y = config().get<float>(sRect+"_y");
            r.width = config().get<float>(sRect+"_w");
            r.height = config().get<float>(sRect+"_h");
            m_visibleAreas.push_back(r);
        }
    }
}

void AreaOfDetection::destroy() {
}

std::vector<lms::math::Rect> AreaOfDetection::visibleAreas(){
    return m_visibleAreas;
}


std::vector<lms::math::Rect> AreaOfDetection::visibleAreas(std::function<void(float,float,float&,float&)> convertPos){
    std::vector<lms::math::Rect> result;
    for(lms::math::Rect &r:m_visibleAreas){
        //float newX;
        //float newY;
        //TODO
    }
    return result;
}

} // namespace area_of_detection

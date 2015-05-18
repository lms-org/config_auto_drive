#include "environment_line_persistent.h"
#include <cmath>
#include <algorithm>
#include "lms/datamanager.h"

bool LinePersistent::initialize() {
    input = datamanager()->readChannel<lms::math::polyLine2f>(this, "POLYLINE_INPUT");
    output = datamanager()->writeChannel<lms::math::polyLine2f>(this, "POLYLINE_OUTPUT");
    frameworkInfo = datamanager()->readChannel<lms::type::FrameworkInfo>(this,"FRAMEWORK_INFO");
    return true;
}


bool LinePersistent::deinitialize() {
    return true;
}

bool LinePersistent::cycle() {
    //add lane to history
    addLine(*input);

    //remove lanes from history
    validateLines();

    //create lane
    createOutput();
    return true;
}

void LinePersistent::addLine(const lms::math::polyLine2f &line){
    LaneStore ls;
    if(line.points().size() < getConfig()->get<uint>("minPointCount",1)){
        return;
    }
    ls.lane = line;
    ls.created = lms::extra::PrecisionTime::now();
    ls.cycleCreated =frameworkInfo->cycleIteration();
    lanes.push_back(ls);
}

void LinePersistent::validateLines(){
    for(uint i = 0; i < lanes.size();){
        LaneStore &ls = lanes[i];
        if(lanes.size() > 2 && frameworkInfo->cycleIteration()-ls.cycleCreated > 5){
            lanes.erase(lanes.begin() + i);
        }else{
            i++;
        }
    }
}

void LinePersistent::createOutput(){
    output->points().clear();
    for(LaneStore &ls: lanes){
        (*output)+= ls.lane;
    }
    output->sort([](const lms::math::vertex2f p1,const lms::math::vertex2f p2){
        return p1.x < p2.x;
    });
}

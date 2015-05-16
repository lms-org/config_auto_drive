#include "image_hint_generator.h"
#include "lms/imaging_detection/line.h"
#include "lms/imaging_detection/line_point.h"
#include "lms/imaging_detection/image_hint.h"

bool ImageHintGenerator::initialize() {
    hintContainer = datamanager()->
            writeChannel<lms::imaging::find::HintContainer>(this,"HINTS");
    target = datamanager()->readChannel<lms::imaging::Image>(this,"TARGET_IMAGE");

    gaussBuffer = new lms::imaging::Image();
    lms::imaging::find::ImageHint<lms::imaging::find::Line> *hint = new lms::imaging::find::ImageHint<lms::imaging::find::Line>();
    hint->name = "RIGHT_LANE";
    hint->parameter.target =target;
    hint->parameter.maxLength = 300;
    hint->parameter.approxEdge = false;
    hint->parameter.lineWidthMax = 10;
    hint->parameter.lineWidthMin = 1;
    hint->parameter.searchAngle = 0;
    hint->parameter.searchLength = 100;
    hint->parameter.gaussBuffer = gaussBuffer;
    hint->parameter.x = 180;
    hint->parameter.y = 120;
    hint->parameter.sobelThreshold = 250;
    hint->parameter.stepLengthMin = 2;
    hint->parameter.stepLengthMax = 20;
    hint->parameter.lineWidthTransMultiplier = 1;
    hint->parameter.edge = false;
    hint->parameter.verify = true;
    hint->parameter.preferVerify = false;
    hint->parameter.validPoint = [](lms::imaging::find::LinePoint &lp)->bool{
        //logger.info("check") << x <<" "<< y;
        bool result =  std::abs(160-lp.high_low.x())>50 || std::abs(lp.high_low.y())<140;
        //result =
        return result;
    };
    //hint->parameter.containing;
    //add it
    hintContainer->add(hint);

    hint = new lms::imaging::find::ImageHint<lms::imaging::find::Line>(*hint);
    hint->name = "LEFT_LANE";
    hint->parameter.x = 80;
    hint->parameter.y = 100;
    hint->parameter.searchAngle = -M_PI;
    hint->parameter.sobelThreshold = 250;
    hint->parameter.lineWidthMin = 0;
    hint->parameter.edge = false;
    hint->parameter.verify = true;
    //hintContainer->add(hint);

    hint = new lms::imaging::find::ImageHint<lms::imaging::find::Line>(*hint);
    hint->name = "BOX";
    hint->parameter.x = 120;
    hint->parameter.y = 50;
    hint->parameter.searchAngle = -M_PI_2l*1.5;
    hint->parameter.stepLengthMax = 5;
    hint->parameter.lineWidthMax = 5;
    hint->parameter.maxLength = 20;
    hint->parameter.edge = false;
    //hintContainer->add(hint);

    return true;
}

const lms::imaging::Image* ImageHintGenerator::getTargetImage(std::string name){
    if(targets.find(name) == targets.end()){
        targets[name] = datamanager()->readChannel<lms::imaging::Image>(this,name);
    }
    return targets[name];

}
bool ImageHintGenerator::deinitialize() {
    return false;
}

bool ImageHintGenerator::cycle() {
    gaussBuffer->resize(target->width(),target->height(),lms::imaging::Format::GREY);
    gaussBuffer->fill(255);
    for(lms::imaging::find::ImageHintBase *ihb : hintContainer->hints){
        lms::imaging::find::ImageHint<lms::imaging::find::Line> *ih =(lms::imaging::find::ImageHint<lms::imaging::find::Line>*) ihb;
        if(ih->imageObject.points().size() < 5){
            ih->parameter.preferVerify = false;
        }
    }
    //TODO calc new good positions (atm will will just use verify in the line etc.)
    return true;
}

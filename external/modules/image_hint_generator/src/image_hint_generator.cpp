#include "image_hint_generator.h"
#include "lms/imaging_detection/line.h"
#include "lms/imaging_detection/line_point.h"
#include "lms/imaging_detection/image_hint.h"
#include "lms/imaging_detection/splitted_line.h"
#include "lms/math/math.h"
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
    hint->parameter.validPoint = [](lms::imaging::find::LinePoint &lp DRAWDEBUG_PARAM)->bool{
        (void)DRAWDEBUG_ARG_N;
        //logger.info("check") << x <<" "<< y;
        bool result =  std::abs(160-lp.high_low.x)>50 || std::abs(lp.high_low.y)<140;
        //result =
        return result;
    };
    //hint->parameter.containing;
    //add it
    //hintContainer->add(hint);

    lms::imaging::find::ImageHint<lms::imaging::find::SplittedLine> *hintSplit = new lms::imaging::find::ImageHint<lms::imaging::find::SplittedLine>();
    hintSplit->name = "MIDDLE_LANE";
    hintSplit->parameter.target =target;
    hintSplit->parameter.maxLength = 300;
    hintSplit->parameter.approxEdge = false;
    hintSplit->parameter.lineWidthMax = 10;
    hintSplit->parameter.lineWidthMin = 1;
    hintSplit->parameter.searchAngle = 0;
    hintSplit->parameter.searchLength = 30;
    hintSplit->parameter.gaussBuffer = gaussBuffer;
    hintSplit->parameter.x = 60;
    hintSplit->parameter.y = 120;
    hintSplit->parameter.sobelThreshold = 250;
    hintSplit->parameter.stepLengthMin = 1;
    hintSplit->parameter.stepLengthMax = 5;
    hintSplit->parameter.lineWidthTransMultiplier = 1;
    hintSplit->parameter.edge = false;
    hintSplit->parameter.verify = true;
    hintSplit->parameter.preferVerify = false;
    hintSplit->parameter.distanceBetween = 40;
    hintSplit->parameter.lineMinLength = 10;
    hintSplit->parameter.lineMaxLength = 40;
    hintSplit->parameter.validPoint = [this](lms::imaging::find::LinePoint &lp DRAWDEBUG_PARAM){
        (void)DRAWDEBUG_ARG_N;
        bool result =  std::abs(160-lp.high_low.x>50) || std::abs(lp.high_low.y)<140;
        float angle = lms::math::limitAngle_nPI_PI(lp.param().searchAngle);
        result = result && (fabs(angle) < M_PI_2l*0.5);
        return result;
    };
    //add it
    hintContainer->add(hintSplit);



    hint = new lms::imaging::find::ImageHint<lms::imaging::find::Line>(*hint);
    hint->name = "BOX";
    hint->parameter.x = 120;
    hint->parameter.y = 50;
    hint->parameter.searchAngle = -M_PI_2l*1.5;
    hint->parameter.stepLengthMax = 5;
    hint->parameter.lineWidthMax = 5;
    hint->parameter.maxLength = 20;
    hint->parameter.edge = true;
    hint->parameter.validPoint = [](lms::imaging::find::LinePoint &lp DRAWDEBUG_PARAM){
        //logger.info("check") << x <<" "<< y;
        lms::imaging::find::EdgePoint::EdgePointParam param = lp.param();
        param.searchType = lms::imaging::find::EdgePoint::EdgeType::HIGH_LOW;
        param.searchLength = 20;
        lms::imaging::find::EdgePoint ep;
        return !ep.find(param DRAWDEBUG_ARG);
    };
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

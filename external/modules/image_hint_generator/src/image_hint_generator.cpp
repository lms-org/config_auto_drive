#include "image_hint_generator.h"
#include "lms/imaging_detection/line.h"
#include "lms/imaging_detection/line_point.h"
#include "lms/imaging_detection/image_hint.h"
#include "lms/imaging_detection/splitted_line.h"
#include "lms/imaging_detection/point_line.h"
#include "lms/math/math.h"
#include "lms/math/vertex.h"
#include "lms/imaging/warp.h"
bool ImageHintGenerator::initialize() {
    gaussBuffer = new lms::imaging::Image();
    middleEnv = datamanager()->writeChannel<Environment>(this,"ENV_MID");
    hintContainer = datamanager()->
            writeChannel<lms::imaging::find::HintContainer>(this,"HINTS");
    target = datamanager()->readChannel<lms::imaging::Image>(this,"TARGET_IMAGE");
    defaultLinePointParameter.fromConfig(getConfig("defaultLPParameter"));
    defaultLinePointParameter.target =target;
    defaultLinePointParameter.gaussBuffer = gaussBuffer;
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
    //TODO
    static bool fromMiddle = true;
    hintContainer->clear();
    //set the gaussbuffer
    gaussBuffer->resize(target->width(),target->height(),lms::imaging::Format::GREY);
    //clear the gaussBuffer not necessary!
    if(fromMiddle){
        createHintsFromMiddleLane();
    }else{
        initialHints();
        fromMiddle = true;
    }
    return true;
}

void ImageHintGenerator::createHintsFromMiddleLane(){
    using lms::math::vertex2f;
    using lms::math::vertex2i;
    if(middleEnv->lanes.size() != 1){
        logger.error("createHintsFromMiddleLane")<<"no valid evironment for middle-lane";
        return;
    }
    const Environment::RoadLane &middle = middleEnv->lanes[0];
    if(middle.type() != Environment::RoadLaneType::MIDDLE){
        logger.error("createHintsFromMiddleLane") << "middle is no middle lane!";
        return;
    }
    float lineDistance = 0.3;
    lms::imaging::find::ImageHint<lms::imaging::find::PointLine> *hintLeft = new lms::imaging::find::ImageHint<lms::imaging::find::PointLine>();
    hintLeft->name = "LEFT_LANE";
    lms::imaging::find::ImageHint<lms::imaging::find::PointLine> *hintRight = new lms::imaging::find::ImageHint<lms::imaging::find::PointLine>();
    hintRight->name = "RIGHT_LANE";
    lms::imaging::find::ImageHint<lms::imaging::find::PointLine> *hintMiddle = new lms::imaging::find::ImageHint<lms::imaging::find::PointLine>();
    hintMiddle->name = "MIDDLE_LANE";
    for(int i = 1; i < (int)middle.points().size(); i++){
        vertex2f bot = middle.points()[i-1];
        vertex2f top = middle.points()[i];
        vertex2f distance = top-bot;
        distance = distance.normalize();
        float tmpX = distance.x;
        distance.x = -distance.y;
        distance.y = tmpX;
        distance *= lineDistance;
        vertex2f left = top+distance;
        vertex2f right = top-distance;
        vertex2f middle = top-distance*0.1;

        vertex2i leftI;
        vertex2i rightI;
        vertex2i middleI;
        vertex2i topI;

        lms::imaging::V2C(&left,&leftI);
        lms::imaging::V2C(&right,&rightI);
        lms::imaging::V2C(&middle,&middleI);
        lms::imaging::V2C(&top,&topI);

        float angleLeft = (leftI-topI).angle();

        //add hints
        //add left
        defaultLinePointParameter.x = leftI.x;
        defaultLinePointParameter.y = leftI.y;
        defaultLinePointParameter.searchAngle = angleLeft;
        hintLeft->parameter.addParam(defaultLinePointParameter);

        //add right
        defaultLinePointParameter.x = rightI.x;
        defaultLinePointParameter.y = rightI.y;
        defaultLinePointParameter.searchAngle = angleLeft+M_PI;
        hintRight->parameter.addParam(defaultLinePointParameter);

        //add middle
        defaultLinePointParameter.x = middleI.x;
        defaultLinePointParameter.y = middleI.y;
        defaultLinePointParameter.searchAngle = angleLeft;
        hintMiddle->parameter.addParam(defaultLinePointParameter);
    }
    hintContainer->add(hintLeft);
    hintContainer->add(hintRight);
    hintContainer->add(hintMiddle);
}

void ImageHintGenerator::initialHints(){
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
    hintContainer->add(hint);
    //hint->parameter.containing;
    //add it
    hint = new lms::imaging::find::ImageHint<lms::imaging::find::Line>(*hint);
    hint->name = "LEFT_LANE";
    hint->parameter.x = 40;
    hint->parameter.y = 100;
    hint->parameter.searchAngle = M_PI;
    hintContainer->add(hint);

    lms::imaging::find::ImageHint<lms::imaging::find::SplittedLine> *hintSplit = new lms::imaging::find::ImageHint<lms::imaging::find::SplittedLine>();
    hintSplit->name = "MIDDLE_LANE";
    hintSplit->parameter.target =target;
    hintSplit->parameter.maxLength = 300;
    hintSplit->parameter.approxEdge = false;
    hintSplit->parameter.lineWidthMax = 10;
    hintSplit->parameter.lineWidthMin = 5;
    hintSplit->parameter.searchAngle = 0;
    hintSplit->parameter.searchLength = 50;
    hintSplit->parameter.gaussBuffer = gaussBuffer;
    hintSplit->parameter.x = 40;
    hintSplit->parameter.y = 210;
    hintSplit->parameter.sobelThreshold = 250;
    hintSplit->parameter.stepLengthMin = 5;
    hintSplit->parameter.stepLengthMax = 10;
    hintSplit->parameter.lineWidthTransMultiplier = 1;
    hintSplit->parameter.edge = false;
    hintSplit->parameter.verify = true;
    hintSplit->parameter.preferVerify = false;
    hintSplit->parameter.distanceBetween = 40;
    hintSplit->parameter.lineMinLength = 10;
    hintSplit->parameter.lineMaxLength = 80;
    hintSplit->parameter.validPoint = [this](lms::imaging::find::LinePoint &lp DRAWDEBUG_PARAM){
        (void)DRAWDEBUG_ARG_N;
        bool result =  std::abs(160-lp.low_high.x)>50 || std::abs(lp.low_high.y)<140;
        float angle = lms::math::limitAngle_nPI_PI(lp.param().searchAngle);
        result = result && (fabs(angle) < M_PI_2l*0.5) &&!(lp.low_high.y < 50);
        return result;
    };
    //TODO atm we can't search for the middle-lane that way (transformer fails)
    //hintContainer->add(hintSplit);



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
}

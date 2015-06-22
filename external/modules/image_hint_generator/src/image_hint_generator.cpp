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
    config = getConfig();
    gaussBuffer = new lms::imaging::Image();
    middleEnv = datamanager()->writeChannel<street_environment::Environment>(this,"ENV_MID");
    hintContainerLane = datamanager()->
            writeChannel<lms::imaging::find::HintContainer>(this,"HINTS");

    hintContainerObstacle = datamanager()->
            writeChannel<lms::imaging::find::HintContainer>(this,"OBSTACLE_HINTS");

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
    static bool fromMiddle = true;
    hintContainerLane->clear();
    hintContainerObstacle->clear();
    //set the gaussbuffer
    gaussBuffer->resize(target->width(),target->height(),lms::imaging::Format::GREY);
    //clear the gaussBuffer not necessary!
    if(fromMiddle){
        if(middleEnv->objects.size() != 1){
            logger.error("createHintsFromMiddleLane")<<"no valid evironment for middle-lane";
            return true;
        }
        const street_environment::RoadLane &middle = middleEnv->objects[0]->getAsReference<const street_environment::RoadLane>();
        if(middle.type() != street_environment::RoadLaneType::MIDDLE){
            logger.error("createHintsFromMiddleLane") << "middle is no middle lane!";
            return true;
        }
        createHintsFromMiddleLane(middle);

        if(config->get<bool>("searchForObstacles",false)){
            createHintForObstacleUsingSinglePoints(middle);
        }else if(config->get<bool>("searchForCrossing",false)){
            createHintForCrossingUsingSinglePoints(middle);
        }
    }else{
        initialHints();
        fromMiddle = true;
    }
    return true;
}

void ImageHintGenerator::createHintForCrossingUsingSinglePoints(const street_environment::RoadLane &middle ){
    using lms::math::vertex2f;
    using lms::math::vertex2i;

    //TODO der code ist schon sehr redundant, da sollte man sich was tolles überlegen!

    float streetWidth = 0.4;
    //float startSearchDistance = 0.4;
    int numberOfSearchPoints = 1;
    //+1 damit man nicht auf der Linie sucht
    float distanceBetweenSearchPoints = streetWidth/(numberOfSearchPoints+1);

    //Abstand zum Auto
    float startSearchDistance = 0.4;
    //suchlänge
    float searchLength = 2;

    bool foundStart = false;
    //aufpunkt auf der linie
    vertex2f *middlePoints = new vertex2f[3*numberOfSearchPoints];
    for(int i = 1; i < (int)middle.points().size(); i++){
        //TODO ziehmlich schlecht so ;D
        vertex2f top = middle.points()[i];
        vertex2f bot = middle.points()[i-1];
        vertex2f distance = top-bot;
        distance = distance.normalize();
        float tmpX = distance.x;
        distance.x = -distance.y;
        distance.y = tmpX;

        if(top.length() > 0.4 && !foundStart){
            //linke Linie
            for(int k = 0; k < numberOfSearchPoints; k++){
                //such-start-punkte in Auto-Koordinaten
                middlePoints[k] = top+distance*distanceBetweenSearchPoints*(k+1);
                foundStart = true;
            }
            //rechte Linie
            for(int k = 0; k < numberOfSearchPoints; k++){
                //such-start-punkte in Auto-Koordinaten
                middlePoints[numberOfSearchPoints+k] = top-distance*distanceBetweenSearchPoints*(k+1);
                foundStart = true;
            }
            //Linie daneben
            for(int k = 0; k < numberOfSearchPoints; k++){
                //such-start-punkte in Auto-Koordinaten
                middlePoints[2*numberOfSearchPoints+k] = top-distance*(streetWidth + distanceBetweenSearchPoints*(k+1));
                foundStart = true;
            }
        }else if(top.length() > startSearchDistance+searchLength){
            //Hier muss man nichts tun, so weit suchen wir keine Hindernisse
            break;
        }else if(top.length() > 0.4){
            lms::imaging::find::ImageHint<lms::imaging::find::PointLine> *line = new lms::imaging::find::ImageHint<lms::imaging::find::PointLine>();
            line->name = "CROSSING_"+std::to_string(i);
            //Linke linie
            for(int k = 0; k < numberOfSearchPoints; k++){
                vertex2f endMiddle = top+distance*distanceBetweenSearchPoints*(k+1);

                //convert to image-pixel-value
                vertex2i startMiddleI;
                vertex2i endMiddleI;
                lms::imaging::V2C(&middlePoints[k],&startMiddleI);
                lms::imaging::V2C(&endMiddle,&endMiddleI);
                //create hint
                float imageSearchDistance = (endMiddleI-startMiddleI).length();
                float searchAngle = (endMiddleI-startMiddleI).angle();

                lms::imaging::find::LinePoint::LinePointParam lpp = defaultLinePointParameter;
                lpp.x = startMiddleI.x;
                lpp.y = startMiddleI.y;
                lpp.searchAngle = searchAngle;
                lpp.searchLength = imageSearchDistance;
                line->parameter.addParam(lpp);
                //alter endPunkt wird neuer Startpunkt:
                middlePoints[k] = endMiddle;
            }
            //rechte Linie
            for(int k = 0; k < numberOfSearchPoints; k++){
                vertex2f endMiddle = top-distance*distanceBetweenSearchPoints*(k+1);

                //convert to image-pixel-value
                vertex2i startMiddleI;
                vertex2i endMiddleI;
                lms::imaging::V2C(&middlePoints[numberOfSearchPoints+k],&startMiddleI);
                lms::imaging::V2C(&endMiddle,&endMiddleI);
                //create hint
                float imageSearchDistance = (endMiddleI-startMiddleI).length();
                float searchAngle = (endMiddleI-startMiddleI).angle();

                lms::imaging::find::LinePoint::LinePointParam lpp = defaultLinePointParameter;
                lpp.x = startMiddleI.x;
                lpp.y = startMiddleI.y;
                lpp.edge = true;
                lpp.searchAngle = searchAngle;
                lpp.searchLength = imageSearchDistance;
                line->parameter.addParam(lpp);
                //alter endPunkt wird neuer Startpunkt:
                middlePoints[numberOfSearchPoints+k] = endMiddle;
            }
            //daneben
            for(int k = 0; k < numberOfSearchPoints; k++){
                vertex2f endMiddle = top-distance*(streetWidth+distanceBetweenSearchPoints*(k+1));

                //convert to image-pixel-value
                vertex2i startMiddleI;
                vertex2i endMiddleI;
                lms::imaging::V2C(&middlePoints[2*numberOfSearchPoints+k],&startMiddleI);
                lms::imaging::V2C(&endMiddle,&endMiddleI);
                //create hint
                float imageSearchDistance = (endMiddleI-startMiddleI).length();
                float searchAngle = (endMiddleI-startMiddleI).angle();

                lms::imaging::find::LinePoint::LinePointParam lpp = defaultLinePointParameter;
                lpp.x = startMiddleI.x;
                lpp.y = startMiddleI.y;
                lpp.edge = true;
                lpp.searchAngle = searchAngle;
                lpp.searchLength = imageSearchDistance;
                line->parameter.addParam(lpp);
                //alter endPunkt wird neuer Startpunkt:
                middlePoints[2*numberOfSearchPoints+k] = endMiddle;
            }
            hintContainerLane->add(line);
        }
    }
    delete[] middlePoints;

}

void ImageHintGenerator::createHintForObstacleUsingOneLineSequence(const street_environment::RoadLane &middle ){
    using lms::math::vertex2f;
    using lms::math::vertex2i;

    lms::imaging::find::Line::LineParam lineParam; //TODO geht sowas?= defaultLinePointParameter;
    lineParam.fromConfig(getConfig("defaultLPParameter"));
    lineParam.target =target;
    lineParam.gaussBuffer = gaussBuffer;
    lineParam.fixedSearchAngle = true;
    lineParam.edge = true;
    //lineWidthMax muss man nur setzen, um die punktverschiebung anzupassen bei der extend menthode der line
    lineParam.lineWidthMax = 10;
    lineParam.lineWidthTransMultiplier = 1;
    lineParam.validPoint = [](lms::imaging::find::LinePoint &lp DRAWDEBUG_PARAM)->bool{
        lms::imaging::find::EdgePoint check = lp.low_high;
        check.searchParam().x = check.x;
        check.searchParam().y = check.y;
        //20 noch in eine Config packen oder irgendwas anderes tolles tun
        check.searchParam().searchLength = 20;
        check.searchParam().searchType = lms::imaging::find::EdgePoint::EdgeType::HIGH_LOW;
        return !check.find(DRAWDEBUG_ARG_N);
    };

    float streetWidth = 0.4;
    //Abstand zum Auto
    float startSearchDistance = 0.4;
    //suchlänge
    float searchLength = 2;

    bool foundStart = false;
    vertex2f startMiddle;
    vertex2f endMiddle;
    for(int i = 1; i < (int)middle.points().size(); i++){
        //TODO ziehmlich schlecht so ;D
        vertex2f bot = middle.points()[i-1];
        vertex2f top = middle.points()[i];
        vertex2f distance = top-bot;
        distance = distance.normalize();
        float tmpX = distance.x;
        distance.x = -distance.y;
        distance.y = tmpX;
        distance *= streetWidth/2;
        if(top.length() > 0.4 && !foundStart){
            //such-start-punkte in Auto-Koordinaten
            startMiddle = top-distance;
            foundStart = true;
        }else if(top.length() > startSearchDistance+searchLength){
            break;
        }else if(top.length() > 0.4){
            endMiddle = top-distance;

            //convert to image-pixel-value
            vertex2i startMiddleI;
            vertex2i endMiddleI;
            lms::imaging::V2C(&startMiddle,&startMiddleI);
            lms::imaging::V2C(&endMiddle,&endMiddleI);
            //create hint
            float imageSearchDistance = (endMiddleI-startMiddleI).length();
            float searchAngle = (endMiddleI-startMiddleI).angle();

            lineParam.searchAngle = searchAngle;
            lineParam.searchLength = imageSearchDistance;
            lineParam.x = startMiddleI.x;
            lineParam.y = startMiddleI.y;
            lineParam.fixedSearchAngle = true;
            //TODO für die 200 noch eine umrechnung nutzen falls nötig!
            lineParam.maxLength =  200;
            lineParam.preferVerify = false;
            lineParam.verify = false;
            lineParam.stepLengthMax = 30;
            lineParam.stepLengthMin = 2;

            /*
             * Besonderheit, die auftreten könnte:
             * Hinderniss wird in zwei Suchen Gefunden, wenn endPunkt und startPunkt der nächsten Linie genau auf der Kante liegen
             */
            lms::imaging::find::ImageHint<lms::imaging::find::Line> *obstHint = new lms::imaging::find::ImageHint<lms::imaging::find::Line>();
            obstHint->name = "OBSTACLE_"+i;
            obstHint->parameter = lineParam;
            hintContainerLane->add(obstHint);

            //alter endPunkt wird neuer Startpunkt:
            startMiddle = endMiddle;
        }
    }

}

void ImageHintGenerator::createHintForObstacleUsingSinglePoints(const street_environment::RoadLane &middle ){
    using lms::math::vertex2f;
    using lms::math::vertex2i;

    //TODO der code ist schon sehr redundant, da sollte man sich was tolles überlegen!

    float streetWidth = 0.4;
    //float startSearchDistance = 0.4;
    int numberOfSearchPoints = 3;
    //vertex2f startPoints[numberOfSearchPoints];
    //+1 damit man nicht auf der Linie sucht
    float distanceBetweenSearchPoints = streetWidth/(numberOfSearchPoints+1);

    //Abstand zum Auto
    float startSearchDistance = 0.4;
    //suchlänge
    float searchLength = 2;

    bool foundStart = false;
    //aufpunkt auf der linie
    vertex2f *middlePoints = new vertex2f[numberOfSearchPoints];
    for(int i = 1; i < (int)middle.points().size(); i++){
        //TODO ziehmlich schlecht so ;D
        vertex2f top = middle.points()[i];
        vertex2f bot = middle.points()[i-1];
        vertex2f distance = top-bot;
        distance = distance.normalize();
        float tmpX = distance.x;
        distance.x = -distance.y;
        distance.y = tmpX;

        if(top.length() > 0.4 && !foundStart){
            for(int k = 0; k < numberOfSearchPoints; k++){
                //TODO auch noch sehr mieser Code
                //such-start-punkte in Auto-Koordinaten
                middlePoints[k] = top-distance*distanceBetweenSearchPoints*(k+1);

                foundStart = true;
            }
        }else if(top.length() > startSearchDistance+searchLength){
            break;
        }else if(top.length() > 0.4){
            lms::imaging::find::ImageHint<lms::imaging::find::PointLine> *line = new lms::imaging::find::ImageHint<lms::imaging::find::PointLine>();
            line->name = "OBSTACLE_"+std::to_string(i);
            line->parameter.validPoint = [](lms::imaging::find::LinePoint &lp DRAWDEBUG_PARAM)->bool{
                lms::imaging::find::EdgePoint check = lp.low_high;
                check.searchParam().x = check.x;
                check.searchParam().y = check.y;
                //20 noch in eine Config packen oder irgendwas anderes tolles tun
                check.searchParam().searchLength = 20;
                check.searchParam().searchType = lms::imaging::find::EdgePoint::EdgeType::HIGH_LOW;
                bool found = check.find(DRAWDEBUG_ARG_N);
                return !found;
            };
            for(int k = 0; k < numberOfSearchPoints; k++){
                vertex2f endMiddle = top-distance*distanceBetweenSearchPoints*(k+1);

                //convert to image-pixel-value
                vertex2i startMiddleI;
                vertex2i endMiddleI;
                lms::imaging::V2C(&middlePoints[k],&startMiddleI);
                lms::imaging::V2C(&endMiddle,&endMiddleI);
                //create hint
                float imageSearchDistance = (endMiddleI-startMiddleI).length();
                float searchAngle = (endMiddleI-startMiddleI).angle();

                lms::imaging::find::LinePoint::LinePointParam lpp = defaultLinePointParameter;
                lpp.x = startMiddleI.x;
                lpp.y = startMiddleI.y;
                lpp.edge = true;
                lpp.searchAngle = searchAngle;
                lpp.searchLength = imageSearchDistance;
                line->parameter.addParam(lpp);
                //alter endPunkt wird neuer Startpunkt:
                middlePoints[k] = endMiddle;
            }
            hintContainerLane->add(line);
        }
    }
    delete[] middlePoints;
}

void ImageHintGenerator::createHintsFromMiddleLane(const street_environment::RoadLane &middle){
    using lms::math::vertex2f;
    using lms::math::vertex2i;
    //line Distance with search offset :)
    float lineOffset = 0.1;
    float lineDistance = 0.4-lineOffset;
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
        //such-start-punkte in Auto-Koordinaten
        vertex2f left = top+distance;
        vertex2f right = top-distance;
        //TODO wenn es nicht mehr geht, dann wegen /lineDistance
        vertex2f middle = top-distance/lineDistance*lineOffset;

        //such-start-punkte in Bild-Koordinaten
        vertex2i leftI;
        vertex2i rightI;
        vertex2i middleI;
        vertex2i topI;


        lms::imaging::V2C(&left,&leftI);
        lms::imaging::V2C(&right,&rightI);
        lms::imaging::V2C(&middle,&middleI);
        lms::imaging::V2C(&top,&topI);

        float angleLeft = (leftI-topI).angle();

        float searchLength = (leftI-topI).length();
        logger.debug("cycle") <<"searchLength pix: "<<searchLength <<" "<< leftI.x << " "<< leftI.y;
        searchLength = searchLength/lineDistance*lineOffset*2;
        logger.debug("cycle")<<"angleLeft: " <<angleLeft << " length: "<<searchLength;

        defaultLinePointParameter.searchLength = searchLength;
        //add hints
        //add left

        if(defaultLinePointParameter.target->inside(leftI.x , leftI.y)){
            defaultLinePointParameter.x = leftI.x;
            defaultLinePointParameter.y = leftI.y;
            defaultLinePointParameter.searchAngle = angleLeft;
            hintLeft->parameter.addParam(defaultLinePointParameter);
        }else{
            logger.debug("cycle")<<"NOT INSIDE - LEFT";
        }

        //add right
        if(defaultLinePointParameter.target->inside(rightI.x , rightI.y)){
            defaultLinePointParameter.x = rightI.x;
            defaultLinePointParameter.y = rightI.y;
            defaultLinePointParameter.searchAngle = angleLeft+M_PI;
            hintRight->parameter.addParam(defaultLinePointParameter);
        }else{
            logger.debug("cycle")<<"NOT INSIDE - RIGHT";
        }

        //add middle
        if(defaultLinePointParameter.target->inside(middleI.x , middleI.y)){
            defaultLinePointParameter.x = middleI.x;
            defaultLinePointParameter.y = middleI.y;
            defaultLinePointParameter.searchAngle = angleLeft;
            hintMiddle->parameter.addParam(defaultLinePointParameter);
        }else{
            logger.debug("cycle")<<"NOT INSIDE - MIDDLE";
        }
    }
    if(hintLeft->getTarget() != nullptr){
        hintContainerLane->add(hintLeft);
    }
    if(hintRight->getTarget() != nullptr){
        hintContainerLane->add(hintRight);
    }
    if(hintMiddle->getTarget() != nullptr){
        hintContainerLane->add(hintMiddle);
    }
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

#if IMAGING_DRAW_DEBUG == 1
        (void)DRAWDEBUG_ARG_N;
#endif
        //logger.info("check") << x <<" "<< y;
        bool result =  std::abs(160-lp.high_low.x)>50 || std::abs(lp.high_low.y)<140;
        //result =
        return result;
    };
    hintContainerLane->add(hint);
    //hint->parameter.containing;
    //add it
    hint = new lms::imaging::find::ImageHint<lms::imaging::find::Line>(*hint);
    hint->name = "LEFT_LANE";
    hint->parameter.x = 40;
    hint->parameter.y = 100;
    hint->parameter.searchAngle = M_PI;
    hintContainerLane->add(hint);

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

#if IMAGING_DRAW_DEBUG == 1
        (void)DRAWDEBUG_ARG_N;
#endif
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

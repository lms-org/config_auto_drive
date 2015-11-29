#include "image_hint_generator.h"
#include "lms/imaging_detection/line.h"
#include "lms/imaging_detection/line_point.h"
#include "lms/imaging_detection/image_hint.h"
#include "lms/imaging_detection/splitted_line.h"
#include "lms/imaging_detection/point_line.h"
#include "lms/math/math.h"
#include <cmath>
#include "lms/math/vertex.h"
#include "lms/imaging_detection/street_crossing.h"
#include "lms/imaging_detection/street_obstacle.h"
#include "lms/imaging/warp.h"
bool ImageHintGenerator::initialize() {
    gaussBuffer = new lms::imaging::Image();
    middleLane = readChannel<street_environment::RoadLane>("MIDDLE_LANE");
    hintContainerLane = writeChannel<lms::imaging::detection::HintContainer>("HINTS");

    hintContainerObstacle = writeChannel<lms::imaging::detection::HintContainer>("HINTS_OBSTACLE");

    target = readChannel<lms::imaging::Image>("TARGET_IMAGE");
    defaultLinePointParameter.target =target.get();
    defaultLinePointParameter.gaussBuffer = gaussBuffer;
    configsChanged();
    return true;
}

void ImageHintGenerator::configsChanged(){
    defaultLinePointParameter.fromConfig(&config("defaultLPParameter"));
    defaultLinePointParameter.fromConfig(&config("defaultEPParameter"));

}

bool ImageHintGenerator::deinitialize() {
    return false;
}
bool ImageHintGenerator::cycle() {
    hintContainerLane->clear();
    hintContainerObstacle->clear();
    //set the gaussbuffer
    gaussBuffer->resize(target->width(),target->height(),lms::imaging::Format::GREY);
    //clear the gaussBuffer not necessary!

        if(middleLane->type() != street_environment::RoadLaneType::MIDDLE){
            logger.error("createHintsFromMiddleLane") << "middle is no middle lane!";
            return true;
        }
        createHintsFromMiddleLane(*middleLane);

        if(config().get<bool>("searchForObstacles",false)){
            createHintForObstacle(*middleLane);
        }else if(config().get<bool>("searchForCrossing",false)){
            createHintForCrossing(*middleLane);
        }

    return true;
}

void ImageHintGenerator::createHintForCrossing(const street_environment::RoadLane &middle ){
    lms::imaging::detection::ImageHint<lms::imaging::detection::StreetCrossing> *crossing = new lms::imaging::detection::ImageHint<lms::imaging::detection::StreetCrossing>();
    lms::imaging::detection::StreetCrossing::StreetCrossingParam scp;
    scp.target = target.get();
    scp.gaussBuffer = gaussBuffer;
    scp.fromConfig(&config("defaultLPParameter"));
    for(const lms::math::vertex2f &v:middle.points()){
        if(v.length() > 0.5 && v.length() < 1.2){
            scp.middleLine.points().push_back(v);
        }
    }
    crossing->parameter = scp;
    hintContainerObstacle->add(crossing);
}

void ImageHintGenerator::createHintForObstacle(const street_environment::RoadLane &middle ){
    lms::imaging::detection::ImageHint<lms::imaging::detection::StreetObstacle> *obstacleRight = new lms::imaging::detection::ImageHint<lms::imaging::detection::StreetObstacle>();
    lms::imaging::detection::ImageHint<lms::imaging::detection::StreetObstacle> *obstacleLeft = new lms::imaging::detection::ImageHint<lms::imaging::detection::StreetObstacle>();

    lms::imaging::detection::StreetObstacle::StreetObstacleParam sopRight;
    lms::imaging::detection::StreetObstacle::StreetObstacleParam sopLeft;
    sopRight.edge = true;
    sopRight.target = target.get();
    sopRight.gaussBuffer = gaussBuffer;
    sopRight.fromConfig(&config("defaultLPParameter"));
    sopRight.fromConfig(&config("defaultObstacleParameter"));
    sopRight.obstacleLeft = false;
    sopRight.middleLine = middle;
    obstacleRight->parameter = sopRight;
    sopLeft = sopRight;
    sopLeft.obstacleLeft = true;
    obstacleLeft->parameter = sopLeft;
    hintContainerObstacle->add(obstacleRight);
    //hintContainerObstacle->add(obstacleLeft);

}

void ImageHintGenerator::createHintForObstacleUsingSinglePoints(const street_environment::RoadLane &middle ){
    using lms::math::vertex2f;
    using lms::math::vertex2i;
    logger.info("createHintForObstacleUsingSinglePoints") << "using";

    //TODO der code ist schon sehr redundant, da sollte man sich was tolles überlegen!

    float streetWidth = 0.4;
    //float startSearchDistance = 0.4;
    int numberOfSearchPoints = 3;
    //vertex2f startPoints[numberOfSearchPoints];
    //+1 damit man nicht auf der Linie sucht
    float distanceBetweenSearchPoints = streetWidth/(numberOfSearchPoints+1);

    //Abstand zum Auto
    float startSearchDistance = 0.6;
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
            lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine> *line = new lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine>();
            line->name = "OBSTACLE_"+std::to_string(i);
            line->parameter.validPoint = [](lms::imaging::detection::LinePoint &lp DRAWDEBUG_PARAM)->bool{
                lms::imaging::detection::EdgePoint check = lp.low_high;
                check.searchParam().x = check.x;
                check.searchParam().y = check.y;
                //20 noch in eine Config packen oder irgendwas anderes tolles tun
                check.searchParam().searchLength = 20;
                check.searchParam().searchType = lms::imaging::detection::EdgePoint::EdgeType::HIGH_LOW;
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

                lms::imaging::detection::LinePoint::LinePointParam lpp = defaultLinePointParameter;
                lpp.x = startMiddleI.x;
                lpp.y = startMiddleI.y;
                lpp.edge = true;
                lpp.searchAngle = searchAngle;
                lpp.searchLength = imageSearchDistance;
                line->parameter.addParam(lpp);
                //alter endPunkt wird neuer Startpunkt:
                middlePoints[k] = endMiddle;
            }
            hintContainerObstacle->add(line);
        }
    }
    delete[] middlePoints;
}

void ImageHintGenerator::createHintsFromMiddleLane(const street_environment::RoadLane &middle){
    //TODO wenn ein Hinderniss einen Suchpunkt überdeckt könnten wir von der anderen Seite danach suchen!
    using lms::math::vertex2f;
    using lms::math::vertex2i;
    //distance between lines and offset for search
    float lineOffset = 0.1;
    float lineDistance = 0.4-lineOffset;
    lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine> *hintLeft = new lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine>();
    hintLeft->name = "LEFT_LANE";
    lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine> *hintRight = new lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine>();
    hintRight->name = "RIGHT_LANE";
    lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine> *hintMiddle = new lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine>();
    hintMiddle->name = "MIDDLE_LANE";
    //TODO: add more random points - punkte nicht nur an den enden erzeugen, abstand angeben und dazwischen interpolieren ,(mindestabstand zwischen den punkten)
    for(int i = 1; i < (int)middle.points().size(); i++){
        const vertex2f bot = middle.points()[i-1];
        const vertex2f top = middle.points()[i];
        const vertex2f tangentMain = (top-bot).normalize();
        const float distance = (top-bot).length();
        const vertex2f distanceOrthNorm = tangentMain.rotateAntiClockwise90deg();

        //number of points per segment
        int numerOfSegments = config().get<int>("numerOfSegmentsLines",1);
        for(int i = 0; i <numerOfSegments; i++){
            lms::math::vertex2f tangent = tangentMain*distance*((float)i)/numerOfSegments;

            //such-start-punkte in Auto-Koordinaten
            vertex2f left = bot+tangent+distanceOrthNorm*lineDistance;
            vertex2f right = bot+tangent-distanceOrthNorm*lineDistance;
            //TODO wenn es nicht mehr geht, dann wegen /lineDistance
            vertex2f middle = bot+tangent-distanceOrthNorm*lineOffset;

            //such-start-punkte in Bild-Koordinaten
            vertex2i leftI;
            vertex2i rightI;
            vertex2i middleI;
            vertex2i topI;


            lms::imaging::V2C(&left,&leftI);
            lms::imaging::V2C(&right,&rightI);
            lms::imaging::V2C(&middle,&middleI);
            lms::imaging::V2C(&top,&topI);

            float angleLeft = (leftI-rightI).angle();

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

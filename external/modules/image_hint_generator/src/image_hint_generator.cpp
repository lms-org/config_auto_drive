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
#include "phoenix_CC2016_service/phoenix_CC2016_service.h"

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
    defaultLinePointParameter.fromConfig(&config("defaultEPParameter"));
    defaultLinePointParameter.fromConfig(&config("defaultLPParameter"));

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
        if(getService<phoenix_CC2016_service::Phoenix_CC2016Service>("PHOENIX_SERVICE")->isValid()){
            if(getService<phoenix_CC2016_service::Phoenix_CC2016Service>("PHOENIX_SERVICE")->driveMode() == phoenix_CC2016_service::CCDriveMode::FMH){
                if(config().get<bool>("searchForObstacles",false)){
                    createHintForObstacle(*middleLane);
                }
                if(config().get<bool>("searchForCrossing",false)){
                    createHintForCrossing(*middleLane);
                }
            }else{
                logger.debug("cycle")<<"Not in FMH: "<<static_cast<int>(getService<phoenix_CC2016_service::Phoenix_CC2016Service>("PHOENIX_SERVICE")->driveMode());
            }
        }else{
            logger.debug("cycle")<<"PHOENIX_SERVICE isn't valid!";
        }

    return true;
}

void ImageHintGenerator::createHintForCrossing(const street_environment::RoadLane &middle ){
    //TODO use cached Crossing to find them
    lms::imaging::detection::ImageHint<lms::imaging::detection::StreetCrossing> *crossing = new lms::imaging::detection::ImageHint<lms::imaging::detection::StreetCrossing>();
    lms::imaging::detection::StreetCrossing::StreetCrossingParam scp;
    scp.target = target.get();
    scp.gaussBuffer = gaussBuffer;;
    scp.fromConfig(&config("defaultEPParameter"));
    scp.fromConfig(&config("defaultLPParameter"));
    scp.fromConfig(&config("defaultLineParameter"));
    scp.lineWidthMax = scp.lineWidthMax*2;
    scp.boxDepthSearchLength = config("defaultCrossingParameter").get<float>("boxDepthSearchLength",20);
    scp.boxPointsNeeded = config("boxPointsNeeded").get<float>("boxPointsNeeded",3);
    const float crossingStartDistance = config("defaultCrossingParameter").get<float>("crossingStartDistance",0.3);
    const float crossingMaxSearchDistance = config("defaultCrossingParameter").get<float>("crossingMaxSearchDistance",1);
    //scp.lineWidthMin = scp.lineWidthMin*2;
    float currentDistance = 0;
    float distanceToStart = crossingStartDistance;
    bool foundFirst = false;
    for(int i = 1; i<(int) middle.points().size(); i++){
        lms::math::vertex2f v1 = middle.points()[i-1];
        lms::math::vertex2f v2 = middle.points()[i];
        currentDistance += v1.distance(v2);
        //Man geht segmentweise vor
        distanceToStart-=v1.distance(v2);
        if(currentDistance < crossingStartDistance || currentDistance > crossingMaxSearchDistance){
            continue;
        }
        if(!foundFirst){
            lms::math::vertex2f d = v2-v1;
            d= d.normalize()*distanceToStart;
            scp.middleLine.points().push_back(v2+d);
            foundFirst = true;
           // logger.error("START")<<v2+d<< v2;
        }
        scp.middleLine.points().push_back(v2);

    }
    crossing->parameter = scp;
    hintContainerObstacle->add(crossing);
}

void ImageHintGenerator::createHintForObstacle(const street_environment::RoadLane &middle ){
    //TODO use cached obstacles to find them
    lms::imaging::detection::ImageHint<lms::imaging::detection::StreetObstacle> *obstacleRight = new lms::imaging::detection::ImageHint<lms::imaging::detection::StreetObstacle>();
    lms::imaging::detection::ImageHint<lms::imaging::detection::StreetObstacle> *obstacleLeft = new lms::imaging::detection::ImageHint<lms::imaging::detection::StreetObstacle>();

    lms::imaging::detection::StreetObstacle::StreetObstacleParam sopRight;
    lms::imaging::detection::StreetObstacle::StreetObstacleParam sopLeft;
    sopRight.edge = true;
    sopRight.target = target.get();
    sopRight.gaussBuffer = gaussBuffer;
    sopRight.fromConfig(&config("defaultEPParameter"));
    sopRight.fromConfig(&config("defaultLPParameter"));
    sopRight.fromConfig(&config("defaultLineParameter"));
    sopRight.fromConfig(&config("defaultObstacleParameter"));
    sopRight.obstacleLeft = false;
    float minObstacleDistanceFromVehicle = config().get<float>("minObstacleDistanceFromVehicle",0.3);
    for(const lms::math::vertex2f &v:middle.points()){
        if(v.x >= minObstacleDistanceFromVehicle){
            sopRight.middleLine.points().push_back(v);
        }
    }
    obstacleRight->parameter = sopRight;
    hintContainerObstacle->add(obstacleRight);
    if(config().get<bool>("searchObstacleLeft",false)){
        sopLeft = sopRight;
        sopLeft.obstacleLeft = true;
        obstacleLeft->parameter = sopLeft;
        hintContainerObstacle->add(obstacleLeft);
    }


}

void ImageHintGenerator::createHintsFromMiddleLane(const street_environment::RoadLane &middle){
    //TODO wenn ein Hinderniss einen Suchpunkt überdeckt könnten wir von der anderen Seite danach suchen!
    using lms::math::vertex2f;
    using lms::math::vertex2i;
    //distance between lines and offset for search
    float lineOffset = config().get<float>("lineOffset",0.1);
    float lineWidth = 0.4;
    float lineDistance = lineWidth-lineOffset;
    float whiteLineWidthMax = config().get<float>("whiteLineWidth",0.02)*config().get<float>("whiteLineErrorMultiplieer",2);
    float whiteLineWidthMin = config().get<float>("whiteLineWidth",0.02)/config().get<float>("whiteLineErrorMultiplieer",2);
    lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine> *hintLeft = new lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine>();
    hintLeft->name = "LEFT_LANE";
    lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine> *hintRight = new lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine>();
    hintRight->name = "RIGHT_LANE";
    lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine> *hintMiddle = new lms::imaging::detection::ImageHint<lms::imaging::detection::PointLine>();
    hintMiddle->name = "MIDDLE_LANE";
    lms::imaging::detection::LinePoint::LinePointParam lpp = defaultLinePointParameter;

    const float maxSearchLength = config().get<float>("maxSearchLength",1);
    float currentSearchLength = 0;
    for(int i = 1; i < (int)middle.points().size(); i++){
        const vertex2f bot = middle.points()[i-1];
        const vertex2f top = middle.points()[i];
        currentSearchLength += bot.distance(top);
        if(currentSearchLength > maxSearchLength){
            break;
        }
        const vertex2f tangentMain = (top-bot).normalize();
        const float distance = (top-bot).length();
        const vertex2f distanceOrthNorm = tangentMain.rotateAntiClockwise90deg();

        //number of points per segment
        const int numerOfSegments = config().get<int>("numerOfSegmentsLines",1);
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
            //set lineWidth

            //could make problems if the values are to small
            lpp.lineWidthMin = searchLength/lineDistance * whiteLineWidthMin;
            lpp.lineWidthMax = searchLength/lineDistance * whiteLineWidthMax;

            logger.debug("cycle") <<"searchLength pix: "<<searchLength <<" "<< leftI.x << " "<< leftI.y;
            searchLength = searchLength/lineDistance*lineOffset*2;
            logger.debug("cycle")<<"angleLeft: " <<angleLeft << " length: "<<searchLength;

            lpp.searchLength = searchLength;
            //add hints
            //add left
            if(lpp.target->inside(leftI.x , leftI.y)){
                lpp.x = leftI.x;
                lpp.y = leftI.y;
                lpp.searchAngle = angleLeft;
                hintLeft->parameter.addParam(lpp);
            }else{
                logger.debug("cycle")<<"NOT INSIDE - LEFT";
            }

            //add right
            if(lpp.target->inside(rightI.x , rightI.y)){
                lpp.x = rightI.x;
                lpp.y = rightI.y;
                lpp.searchAngle = angleLeft+M_PI;
                hintRight->parameter.addParam(lpp);
            }else{
                logger.debug("cycle")<<"NOT INSIDE - RIGHT";
            }

            //add middle
            if(lpp.target->inside(middleI.x , middleI.y)){
                lpp.x = middleI.x;
                lpp.y = middleI.y;
                lpp.searchAngle = angleLeft;
                hintMiddle->parameter.addParam(lpp);
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

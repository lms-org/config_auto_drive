#ifndef IMAGE_HINT_TRANSFORMER_H
#define IMAGE_HINT_TRANSFORMER_H

#include "lms/module.h"
#include "lms/datamanager.h"
#include "lms/imaging_detection/image_hint.h"
#include "street_environment/road.h"
#include "lms/imaging_detection/line_point.h"
#include "lms/type/module_config.h"
#include <vector>

class ImageHintGenerator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
    void configsChanged() override;
private:
    void createHintsFromMiddleLane(const street_environment::RoadLane &middle );
    void createHintForObstacleUsingSinglePoints(const street_environment::RoadLane &middle );
    void createHintForCrossing(const street_environment::RoadLane &middle );
    void createHintForObstacle(const street_environment::RoadLane &middle );
    lms::ReadDataChannel<street_environment::RoadLane> middleLane;
    lms::WriteDataChannel<lms::imaging::detection::HintContainer> hintContainerLane;
    lms::WriteDataChannel<lms::imaging::detection::HintContainer> hintContainerObstacle;
    lms::imaging::Image* gaussBuffer;

    lms::ReadDataChannel<lms::imaging::Image> target;
    //that's not that nice...const
    //std::map<std::string,const lms::imaging::Image*> targets;
    lms::imaging::detection::LinePoint::LinePointParam defaultLinePointParameter;
};

#endif /* IMAGE_HINT_TRANSFORMER_H */

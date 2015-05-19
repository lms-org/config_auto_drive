#include "environment_line_predictor.h"
#include "lms/math/vertex.h"

bool EnvironmentLinePredictor::initialize() {
    input = datamanager()->readChannel<Environment>(this, "ENVIRONMENT_INPUT");
    output = datamanager()->writeChannel<Environment>(this, "ENVIRONMENT_OUTPUT");

    return true;
}

bool EnvironmentLinePredictor::deinitialize() {
    return true;
}

bool EnvironmentLinePredictor::cycle() {
    output->lanes.clear();
    logger.error("CYCLE: ") << input->lanes.size();
    for(const Environment::RoadLane &lane : input->lanes) {
        if(lane.type() == Environment::RoadLaneType::MIDDLE) {
            Environment::RoadLane outputLane;
            outputLane.type(Environment::RoadLaneType::MIDDLE);

            predict(lane, outputLane);

            output->lanes.push_back(outputLane);
        }
    }

    return true;
}

void EnvironmentLinePredictor::predict(const Environment::RoadLane &input,
                                       Environment::RoadLane &output) {
    using lms::math::vertex2f;

    // copy all existing points
    output.points() = input.points();

    if(output.points().size() >= 2) {
        vertex2f along = output.points()[output.points().size() - 1] -
                output.points()[output.points().size() - 2];

        //logger.info("PREDICT") << along.length();

        along = along / (along.length() / 1);

         //logger.info("PREDICT AFTER") << along.x() << " " << along.y();

        vertex2f predictedPoint = output.points()[output.points().size() - 1] +
                along;

        output.points().push_back(predictedPoint);
    }
}

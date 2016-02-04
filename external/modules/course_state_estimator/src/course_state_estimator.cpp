#include "course_state_estimator.h"

bool CourseStateEstimator::initialize() {

    road = readChannel<street_environment::RoadLane>("ROAD");

    observationProbability1 = config().get<float>("observationProbability1", 0.8);
    observationProbability2 = config().get<float>("observationProbability2", 0.7);

    distance1 = config().get<float>("distance1", 0.3);
    distance2 = config().get<float>("distance2", 0.8);
    distance3 = config().get<float>("distance3", 1.3);
    distance4 = config().get<float>("distance4", 1.6);

    curvatureThreshold = config().get<float>("curvatureThreshold", 0.2);

    float transitionProbability = config().get<float>("transitionProbability", 0.01);
    std::cout << transitionProbability << std::endl;

    transition << 1-transitionProbability, 0, transitionProbability,
                  transitionProbability, 1-transitionProbability, 0,
                  0, transitionProbability, 1-transitionProbability;

    states << 1.0,1.0,1.0;
    states /= states.sum();

    return true;
}

bool CourseStateEstimator::deinitialize() {
    return true;
}

bool CourseStateEstimator::cycle() {
    update();
    logger.debug("States:") << states(0) << " " << states(1) << " " << states(2);
    return true;
}


void CourseStateEstimator::update()
{
    mapObservations();
    states = transition*states;
    states = states.cwiseProduct(observation);
    states /= states.sum();
}

void CourseStateEstimator::mapObservations()
{
    float curvature1 = calculateCurvature(distance1, distance2);
    float curvature2 = calculateCurvature(distance3, distance4);

    logger.debug("Curvature:") << curvature1 << "," << curvature2;

    Eigen::Vector2f p1(0.5,0.5);
    mapObservationProbability(curvature1, p1, observationProbability1);

    Eigen::Vector2f p2(0.5,0.5);
    mapObservationProbability(curvature2, p2, observationProbability2);

    observation = Eigen::Vector3f(p1(0)*p2(0), p1(0)*p2(1), p1(1)*p2(1));
}

void CourseStateEstimator::mapObservationProbability(const float& curvature, Eigen::Vector2f& probability, float observationProbability)
{
    //! curve
    if(fabs(curvature) > curvatureThreshold)
    {
        probability = Eigen::Vector2f(1-observationProbability,observationProbability);
    }
    //! straight
    else
    {
        probability = Eigen::Vector2f(observationProbability,1-observationProbability);
    }

}

float CourseStateEstimator::calculateCurvature(float minDistance, float maxDistance)
{
    //=================================================================
    // Here is the place to implement higher level velocity adjustments
    //=================================================================
    int nPointsRoad = road->points().size();
    float lengthEnvModelSegment = road->polarPartLength;

    //check if environment model has needed range
    float maxDistanceEnvModel =  lengthEnvModelSegment * (nPointsRoad-1);
    if (maxDistance > maxDistanceEnvModel)
    {
        logger.warn("max distance is bigger than max distance of environment model");
        maxDistance = maxDistanceEnvModel;
    }
    if (minDistance < 0)
    {
        logger.warn("min distance smaller 0");
        minDistance = 0;
    }

    //calc all points
    lms::math::vertex2f nearestPoint = interpolateRoadAtDistance(minDistance);
    lms::math::vertex2f midPoint = interpolateRoadAtDistance((minDistance+maxDistance)/2);
    lms::math::vertex2f farthestPoint = interpolateRoadAtDistance(maxDistance);

    // calc. curvature opf the circle with signum
    return lms::math::circleCurvature(nearestPoint, midPoint, farthestPoint);
}


lms::math::vertex2f CourseStateEstimator::interpolateRoadAtDistance(float distanceIn)
{
    lms::math::vertex2f result;

    int nPointsRoad = road->points().size();
    float lengthEnvModelSegment = road->polarPartLength;

    float distanceInClean = distanceIn;

    //check if environment model has needed range
    float maxDistanceEnvModel =  lengthEnvModelSegment * (nPointsRoad-1);
    if (distanceInClean >= maxDistanceEnvModel)
    {
        logger.warn("distanceIn is bigger than max distance of environment model");
        distanceInClean = maxDistanceEnvModel;
        result = road->points()[nPointsRoad-1];
        return result;

    }
    if (distanceInClean < 0)
    {
        logger.warn("distanceIn smaller 0");
        distanceInClean = 0;
    }

    // get the point
    if (fmod(distanceInClean, lengthEnvModelSegment) == 0)
    {
        // by chance got one point
        int idPoint = round(distanceInClean/lengthEnvModelSegment);
        result = road->points()[idPoint];
//        logger.warn("perfect hit: i point:  ") << idPoint << ",  distance in: " << distanceInClean;
        return result;
    }

    // is between two points
//    logger.warn("distanceIn   ") << distanceInClean;
    int idPointBefore = floor(distanceInClean/lengthEnvModelSegment);

    if ((idPointBefore < 0) || (idPointBefore > nPointsRoad - 2))
    {

        if (idPointBefore < 0)
        {
            logger.warn("the id of the point selected is smaller 0");
            idPointBefore = 0;
        }
        if (idPointBefore > nPointsRoad - 2)
        {
            logger.warn("the id of the point selected is to big: ") << idPointBefore;
            logger.warn("nPointsRoad: ") << nPointsRoad;
            idPointBefore = nPointsRoad - 2;
        }
    }

    lms::math::vertex2f pointBefore = road->points()[idPointBefore];
    lms::math::vertex2f pointAfter = road->points()[idPointBefore+1]; // not going out of bounds should be automatically detected before


    float fractionFirst =1 - (distanceInClean - idPointBefore*lengthEnvModelSegment)/lengthEnvModelSegment;

    if ((fractionFirst < 0))
    {
        logger.warn("fraction should be bigger 0");
        fractionFirst = 0;
    }
    if ((fractionFirst > 1))
    {
        logger.warn("fraction should be smaller 1");
        fractionFirst = 1;
    }



    result.x = fractionFirst*pointBefore.x + (1-fractionFirst)*pointAfter.x;
    result.y = fractionFirst*pointBefore.y + (1-fractionFirst)*pointAfter.y;

    //logger.warn("i: ") << idPointBefore << ",  distance in: " << distanceInClean << ",  fraction first: " << fractionFirst <<",  x: " << result.x <<",  y= " << result.y << ", point before: x:" << pointBefore.x << ", y:" << pointBefore.y <<",  point0:x " << road->points()[0].x << ", y:" << road->points()[0].y;

    return result;
}


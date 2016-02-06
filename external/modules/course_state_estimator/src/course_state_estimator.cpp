#include "course_state_estimator.h"

bool CourseStateEstimator::initialize() {
    environment = readChannel<street_environment::EnvironmentObjects>("ENVIRONMENT");
    roadStates= writeChannel<street_environment::RoadStates>("ROAD_STATES");

    road = readChannel<street_environment::RoadLane>("ROAD");  

    probabilityStates << 1.0,1.0,1.0;
    probabilityStates /= probabilityStates.sum();
    return true;
}

bool CourseStateEstimator::deinitialize() {
    return true;
}

bool CourseStateEstimator::cycle() {

    observationProbability1 = config().get<float>("observationProbability1", 0.8);
    observationProbability2 = config().get<float>("observationProbability2", 0.7);

    distance1 = config().get<float>("distance1", 0.3);
    distance2 = config().get<float>("distance2", 0.8);
    distance3 = config().get<float>("distance3", 1.3);
    distance4 = config().get<float>("distance4", 1.6);

    curvatureThreshold = config().get<float>("curvatureThreshold", 0.2);

    float transitionProbability1 = config().get<float>("transitionProbabilityStraightToStraightCurve", 0.01);
    float transitionProbability2 = config().get<float>("transitionProbabilityStraightCurveToCurve", 0.01);
    float transitionProbability3 = config().get<float>("transitionProbabilityCurveToStraight", 0.01);

    //! straight straight-curve curve
    transition << 1-transitionProbability1,  0,                        transitionProbability3,
                  transitionProbability1,    1-transitionProbability2, 0,
                  0,                         transitionProbability2,   1-transitionProbability3;

    for(street_environment::EnvironmentObjectPtr envPtr: environment->objects){
        lms::math::polyLine2f allPoints;
        if(envPtr->getType() == street_environment::RoadLane::TYPE){
            street_environment::RoadLanePtr roadPtr = std::static_pointer_cast<street_environment::RoadLane>(envPtr);
            if(roadPtr->points().size() == 0)
                continue;
            lms::math::vertex2f centerOfMass = lms::math::vertex2f(0,0);
            for(const lms::math::vertex2f &v: roadPtr->points()){
                centerOfMass =centerOfMass+ v;
            }
            centerOfMass = centerOfMass/roadPtr->points().size();

            for(const lms::math::vertex2f &v: roadPtr->points()){
                allPoints.points().push_back(v-centerOfMass);
            }
        }
    }
    update();


    logger.debug("States:") << probabilityStates(0) << " " << probabilityStates(1) << " " << probabilityStates(2);
    Eigen::Vector3f probsStraight = CourseStateEstimator::emissionProbabilitiesStraight();
    logger.debug("obs prob:") << probsStraight(0) << " "<< probsStraight(1) << " "<< probsStraight(2);
    roadStates->states.clear();
    for(int i = 0; i < probabilityStates.rows(); i++){
        roadStates->states.push_back(getStateFromIndex(i));
    }

    return true;
}

Eigen::Vector3f CourseStateEstimator::emissionProbabilitiesStraight()
{
    Eigen::Vector3f result;
    result << 1,1,1;
    result = result / result.sum();

    float trustDistance = config().get<float>("trustDistance", 1);

    lms::math::polyLine2f allPointsTrusted;
    lms::math::polyLine2f allPointsUntrusted;

    int localCounter = 0;
    bool flagCenter = false;

    for(street_environment::EnvironmentObjectPtr envPtr: environment->objects){

        if(envPtr->getType() == street_environment::RoadLane::TYPE){
            street_environment::RoadLanePtr roadPtr = std::static_pointer_cast<street_environment::RoadLane>(envPtr);
            if(roadPtr->points().size() == 0)
                continue;

            lms::math::vertex2f centerOfMass = lms::math::vertex2f(0,0);
            flagCenter = false;
            localCounter = 0;

            for(const lms::math::vertex2f &v: roadPtr->points()){
                if (v.x <= trustDistance)
                {
                    centerOfMass = centerOfMass+ v;
                    flagCenter = true;
                    localCounter++;
                }
            }
            if (!flagCenter)
            {
                // not enough points
                continue;
            }


            centerOfMass = centerOfMass/localCounter;

            for(const lms::math::vertex2f &v: roadPtr->points()){
                if (v.x <= trustDistance)
                {
                    //trusted point
                    allPointsTrusted.points().push_back(v - centerOfMass);
                }else
                {
                    allPointsUntrusted.points().push_back(v - centerOfMass);
                }

            }
        }
    }


    // construct cov matrix
    Eigen::Matrix2f covMat;

    covMat<<0,0,
            0,0;

    Eigen::Vector2f currentVec;

    int nPointsTrusted = allPointsTrusted.points().size();
    int nPointsUntrusted = allPointsUntrusted.points().size();

    if (nPointsTrusted+nPointsUntrusted < config().get<int>("minNumberPointsForStraightEstimation", 8))
    {
        logger.warn("number of points for straight estimation not enough");
        return result;
    }

    for (int i = 0; i < nPointsTrusted; i++)
    {
        currentVec(0) = allPointsTrusted.points()[i].x;
        currentVec(1) = allPointsTrusted.points()[i].y;

        covMat += currentVec*currentVec.transpose();
    }

    covMat = covMat / nPointsTrusted;
    covMat = 0.5*(covMat+covMat.transpose());

    //eigendecomposition
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix2f> eigensolver(covMat);

    if (eigensolver.info() != Eigen::Success)
    {
        logger.warn("Eigendecomposition didnt work");
        return result;
    }

    //select Ev with bigger eigenvalue
    Eigen::Vector2f eigenvalues = eigensolver.eigenvalues();
    Eigen::Matrix2f eigenvectors = eigensolver.eigenvectors();

    Eigen::Vector2f dirVector;
    if (eigenvalues(0) > eigenvalues(1))
    {
        if (eigenvalues(0) <= 0)
        {
            logger.warn("ev is negative");
            return result;
        }
        dirVector = eigenvectors.col(0);
    } else
    {
        if (eigenvalues(1) <= 0)
        {
            logger.warn("ev is negative");
            return result;
        }
        dirVector = eigenvectors.col(1);
    }

    //normalize
    dirVector.normalize();

    Eigen::Vector2f normalVector;
    normalVector(0) = -dirVector(1);
    normalVector(1) = dirVector(0);

    normalVector.normalize();

    // count outliers
    float distanceOutlier = config().get<float>("distanceOutlier", 0.05);
    float distanceLocalPoint = 0;

    int numOutliersTrustedRegion = 0;
    int numOutliersUntrustedRegion = 0;

    for (int i = 0; i < nPointsTrusted; i++)
    {
        currentVec(0) = allPointsTrusted.points()[i].x;
        currentVec(1) = allPointsTrusted.points()[i].y;

        distanceLocalPoint = fabs( normalVector.transpose() * currentVec );

        if (distanceLocalPoint > distanceOutlier)
        {
            numOutliersTrustedRegion++;
        }
    }

    for (int i = 0; i < nPointsUntrusted; i++)
    {
        currentVec(0) = allPointsUntrusted.points()[i].x;
        currentVec(1) = allPointsUntrusted.points()[i].y;

        distanceLocalPoint = fabs( normalVector.transpose() * currentVec );

        if (distanceLocalPoint > distanceOutlier)
        {
            numOutliersUntrustedRegion++;
        }
    }

    // now the number of outliers is given
    // put distribution

    float fracOutliersSameProbTrustedRegion = config().get<float>("nOutliersSameProbTrustedRegion", 0.07);
    float fracOutliersSameProbUntrustedRegion = config().get<float>("fracOutliersSameProbUntrustedRegion", 0.1);

    if (nPointsTrusted < 1)
    {
        logger.warn("nPointsTrusted < 1");
        return result;
    }
    if (nPointsUntrusted < 1)
    {
        logger.warn("nPointsUntrusted < 1");
        return result;
    }

    // calculate the ps with formula
    float pTrust = 1-powf(1/(1.f+nPointsTrusted), 1/(fracOutliersSameProbTrustedRegion*nPointsTrusted));
    float pUntrusted = 1-powf(1/(1.f+nPointsUntrusted), 1/(fracOutliersSameProbUntrustedRegion*nPointsUntrusted));

    // claculate elementary probs
    float probStraightTrusted = pTrust*powf(1.f-pTrust, numOutliersTrustedRegion);
    float probCurveTrusted = 1/(nPointsTrusted+1.f);
    float probStraightUntrusted = pUntrusted*powf(1.f-pUntrusted, numOutliersUntrustedRegion);
    float probCurveUntrusted = 1/(nPointsUntrusted+1.f);

    // assemble final probs vector
    result(0) = probStraightTrusted * probStraightUntrusted;
    result(1) = probStraightTrusted * probCurveUntrusted;
    result(2) = probCurveTrusted * probCurveUntrusted;

    result = result / result.sum();

    return result;
}

street_environment::RoadState CourseStateEstimator::getStateFromIndex(int index){
    street_environment::RoadState rState;
    switch (index) {
    case 0:
        rState.type = street_environment::RoadStateType::STRAIGHT;
        break;
    case 1:
        rState.type = street_environment::RoadStateType::STRAIGHT_CURVE;
        break;
    case 2:
        rState.type = street_environment::RoadStateType::CURVE;
        break;
    default:
        rState.type = street_environment::RoadStateType::UNKNOWN;
        break;
    }
    rState.probability = probabilityStates(index);
    rState.curvature = curvaturePT1;
    return rState;
}


void CourseStateEstimator::update()
{

    if (config().get<bool>("useKalmanCurvature", true))
    {
        //update curvature
        float curv = 0.0;
        for (uint i=4; i <= 6; ++i)
        {
            curv += road->polarDarstellung[i];
        }
        curv /= 3.0;

        //Unterscheidung ob "innnen" oder "aussen" in der Kurve
        if (curv > 0) //Linkskurve
        {
            curv = curv/(1.0 + 0.2*curv);
        }
        else
        {
            curv = curv/(1.0 - 0.2*curv);
        }

        curvaturePT1 = curv;

    }
    else
    {
        //update curvature
        float pt1_parameter = config().get<float>("curvaturePT1_parameter", 0.05);
        curvaturePT1 = pt1_parameter*calculateCurvature(0.2, 1.2) + (1-pt1_parameter)*curvaturePT1;


    }
    mapObservations();
    probabilityStates = transition*probabilityStates;
    probabilityStates = probabilityStates.cwiseProduct(observation);
    if (config().get<bool>("useProbabilitiesStraight", false)) probabilityStates = probabilityStates.cwiseProduct(emissionProbabilitiesStraight());
    probabilityStates /= probabilityStates.sum();


}

void CourseStateEstimator::mapObservations()
{
    float curvature1 = calculateCurvature(distance1, distance2);
    float curvature2 = calculateCurvature(distance3, distance4);

    //logger.debug("Curvature:") << curvature1 << "," << curvature2;

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
    lms::math::vertex2f nearestPoint = road->interpolateAtDistance(minDistance);
    lms::math::vertex2f midPoint = road->interpolateAtDistance((minDistance+maxDistance)/2);
    lms::math::vertex2f farthestPoint = road->interpolateAtDistance(maxDistance);

    // calc. curvature opf the circle with signum
    return lms::math::circleCurvature(nearestPoint, midPoint, farthestPoint);
}

#include "local_course/local_course.h"

#include <filter/line_line_x.h>
#include <unistd.h>
#include <filter/adam.h>
#include <cmath>

namespace local_course {

LocalCourse::LocalCourse():kalman(logger){

}

bool LocalCourse::init() {
    lineX = new LineX();
    configsChanged();
    return true;
}

void LocalCourse::destroy() {
    //TODO
}

void LocalCourse::configsChanged(){
    lineX->init(config().get<int>("elementCount",10));
    lineX->state(1)=config().get<float>("yOffset",0.2); //set y-offset
    lineX->lineLength = config().get<float>("elementLength",0.2);
    lineX->fixX = config().get<float>("fixX",true);
    lineX->fixY = config().get<float>("fixY",false);

    outlierStartingState = config().get<int>("outlierStartingPoint", 1);
    outlierPercentile = config().get<float>("outlierPercentile", 0.5);
    outlierPercentileMultiplier = config().get<float>("outlierPercentileMultiplier", 3.0);

    m_thresholdLookup.vx = config().getArray<float>("thresholdLutX");
    m_thresholdLookup.vy = config().getArray<float>("thresholdLutY");
}

void LocalCourse::filterPoints(){
    street_environment::RoadLane lane = getCourse(); //calculate xy points of lane
    //remove first points if they are to far away (lazy init search)
    if (++resetCounter > 40){

        if (config().get<bool>("useThresholding", true)){

            std::vector< std::tuple<int,float, float> > thresholdData;
            std::tuple<int,float, float>  d (0,1000.0, 0); //index, distance, lambda
            thresholdData.assign(pointsToAdd.size(), d);

            float dst;
            float lambda;
            float delta = 0.2; //length of a lane segment

            for (int s = 0; s < (int)lane.points().size(); ++s)
            {
                for (int m = 0; m < (int) pointsToAdd.size(); ++m)
                {
                    dst = pointsToAdd[m].distance(lane.points()[s]);
                    if (dst < std::get<1>(thresholdData.at(m)))
                    {
                        std::get<0>(thresholdData.at(m)) = s;
                        std::get<1>(thresholdData.at(m)) = dst;
                    }

                    if (s>0)
                    {
                        distanceLinePoint(lane.points()[s-1], lane.points()[s], pointsToAdd[m], &dst, &lambda);

                        if (dst < std::get<1>(thresholdData.at(m)) && lambda > 0 && lambda < 1)
                        {
                            std::get<0>(thresholdData.at(m)) = s;
                            std::get<1>(thresholdData.at(m)) = dst;
                            std::get<2>(thresholdData.at(m)) = lambda;
                        }
                    }
                }
            }

            std::vector<int> indices;

            for (int i = 0; i <  (int)thresholdData.size(); ++i)
            {

                //thresholding
                float s = delta * (std::get<0>(thresholdData.at(i)) + std::get<2>(thresholdData.at(i))); //arc length
                float maxAllowedDistance = thresholdFunction(s);
                if (std::get<1>(thresholdData.at(i)) > maxAllowedDistance)
                {
                    indices.push_back(i);
                }
            }

            std::sort(indices.begin(), indices.end());

            for (int i = indices.size()-1; i >= 0; --i)
            {
                pointsToAdd.erase(pointsToAdd.begin() + indices[i]);
            }

        }
        else
        {
            std::vector< std::pair<int, float> > data; //index of nearest state-point and distance to it

            std::pair<int, float> pair(0, 10000.0);
            data.assign(pointsToAdd.size(), pair);

            float dst;
            float lambda;

            for (int s = 0; s < lane.points().size(); ++s)
            {
                for (int m = 0; m < pointsToAdd.size(); ++m)
                {
                    dst = pointsToAdd[m].distance(lane.points()[s]);
                    if (dst < data.at(m).second)
                    {
                        data.at(m).first = s; //index of state-point nearest to the measurement point
                        data.at(m).second = dst;
                    }

                    if (s>0)
                    {
                        distanceLinePoint(lane.points()[s-1], lane.points()[s], pointsToAdd[m], &dst, &lambda);

                        if (dst < data.at(m).second && lambda > 0 && lambda < 1)
                        {
                            data.at(m).first = s-1;
                            data.at(m).second = dst;
                        }
                    }
                }
            }

            std::vector<float> observations;
            std::vector<int> indices;

            for (int i = 0; i < data.size(); ++i)
            {
                if (data[i].first >= outlierStartingState)
                {
                    observations.push_back(data[i].second);
                }
            }

            if (observations.size() > 0)
            {
                float perc = outlierPercentile;
                std::nth_element(observations.begin(), observations.begin() + perc*observations.size(), observations.end());
                float percVal = observations[perc*observations.size()];
                float maxDistance = outlierPercentileMultiplier*percVal;


                for (int i = 0; i <data.size();)
                {
                    if (data[i].first >= outlierStartingState && data[i].second > maxDistance)
                    {
                        pointsToAdd.erase(pointsToAdd.begin() + i);
                        data.erase(data.begin() + i);

                    }else{
                        i++;
                    }
                }
            }
        }
    }
}


void LocalCourse::update(float dx, float dy, float dphi){
    //remove outliers TODO move to other module/function!
    lineX->translate(dx,dy,dphi);
    for(int i = 0; i < 20; i++){
        for(int row = 0; row < (int)pointsToAdd.size(); row++){
            lineX->update(Eigen::Vector2d(pointsToAdd[row].x,pointsToAdd[row].y));
        }
    }
    //lineX->state(lineX->state.rows()-1) = lineX->state(lineX->state.rows()-2);
    pointsAdded = pointsToAdd;
    pointsToAdd.clear();
}

std::vector<lms::math::vertex2f> LocalCourse::getPointsToAdd(){
    return pointsToAdd;
}

std::vector<lms::math::vertex2f> LocalCourse::getPointsAdded(){
    return pointsAdded;
}


void LocalCourse::resetData(){
    //resetCounter = 0;
    pointsToAdd.clear();
    pointsAdded.clear();
    configsChanged();
    //kalman.resetData(config());
}


void LocalCourse::addPoints(const std::vector<lms::math::vertex2f> &points){
    for(const lms::math::vertex2f &v :points){
        if (!(std::isnan(v.x) || std::isnan(v.y)))  pointsToAdd.push_back(v);// TODO nan??
    }
}

void LocalCourse::addPoint(const lms::math::vertex2f &p){
    if (!(std::isnan(p.x) || std::isnan(p.y)))  pointsToAdd.push_back(p); // TODO nan??
}



street_environment::RoadLane LocalCourse::getCourse(){
    Eigen::Matrix<double,Eigen::Dynamic,2> data = lineX->toXY();

    street_environment::RoadLane r;
    for(int row = 0; row < data.rows(); row++){
        r.points().push_back(lms::math::vertex2f(data(row,0),data(row,1)));
    }
    return r;
    //return kalman.getOutput();
}


street_environment::RoadLane LocalCourse::getCourse(lms::Time time){
    (void)time;
    logger.error("DOES NOT WORK");
    LMS_EXCEPTION("NOT IMPLEMENTED YET!");
    //TODO
    street_environment::RoadLane r;
    return r;
}

float LocalCourse::thresholdFunction(float s){
    //lut
    return m_thresholdLookup.linearSearch(s);

}

void LocalCourse::distanceLinePoint(lms::math::vertex2f P, lms::math::vertex2f Q, lms::math::vertex2f M, float *dst, float *lambda){
    //calculate distance of M from the line connecting P and Q
    //lambda = 0, if the orthogonal line from PQ to M goes through P
    //lambda = 1, if the orthogonal line from PQ to M goes through Q
    //lambda  [0, ... , 1] inbetween

    lms::math::vertex2f V = Q - P;

    *lambda = - (V.x*(P.x-M.x) + V.y*(P.y-M.y))/(V.x*V.x + V.y*V.y);
    lms::math::vertex2f S = P + V*(*lambda);

    *dst = sqrt((M.x-S.x)*(M.x-S.x) + (M.y-S.y)*(M.y-S.y));
}

} // namespace local_course

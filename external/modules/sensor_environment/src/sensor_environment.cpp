#include <sensor_environment/sensor_environment.h>
#include <cmath>

LinedDepthEnvironment::LinedDepthEnvironment(float stepLength, float sensivity):
    m_stepLength(stepLength),m_sensivity(sensivity),m_minY(-INFINITY), m_maxY(INFINITY){
}

bool LinedDepthEnvironment::add(float deltaX,float y){
    xBuffer += deltaX;
    if(xBuffer < m_stepLength)
        return false;
    //limit the y-values
    if(y < m_minY)
        y = m_minY;
    if(y > m_maxY)
        y = m_maxY;
    //add values to the discrete vector
    while(xBuffer > m_stepLength){
        m_distances.push_back(y);
        xBuffer -= m_stepLength;
    }
    //now the xBuffer should be negative as we substracted the stepLength one time to often -> add it again
    xBuffer += m_stepLength;
    return true;
}

/**
 * @brief draw
 * @param distanceInPixel how many meters one pixel is
 */
void LinedDepthEnvironment::draw(lms::imaging::Graphics *graphics,float distanceInPixel){
    float mul = distanceInPixel*m_stepLength;
    int xPadding = 20;
    graphics->setColor(lms::imaging::black);
    //draw all points
    for(int i = 0; i < m_distances.size()-1; i++){
        graphics->drawLine(xPadding+m_distances[i]*mul,i*mul,xPadding+m_distances[i+1]*mul,(i+1)*mul);
    }
    //draw found parts
    //might be disturbing that the x-axis of the part is the y-axis of the image :)
    graphics->setColor(lms::imaging::blue);
    for(int i = 0; i < parts.size(); i++){
        LineDepthSegment &p = parts[i];
        graphics->drawLine(xPadding+p.y*mul,p.startIndex*mul,xPadding + p.y*mul,p.endIndex*mul);
    }
}

void LinedDepthEnvironment::validate(){
    if(m_sensivity == 0 || m_distances.size() == 0){
        //TODO not sure what should happen :'(
        return;
    }
    int startSearch = 0;
    if(parts.size() > 0){
        startSearch = parts[parts.size()-1].endIndex+1;
    }
    float startY = m_distances[startSearch];

    for(int i = startSearch+1; i < m_distances.size(); i++){
        if(abs(m_distances[i]-startY) > m_sensivity){
            //found new part
            parts.push_back(LineDepthSegment(startSearch,i,startSearch*m_stepLength, i*m_stepLength,startY));
        }
    }
}


const std::vector<LinedDepthEnvironment::LineDepthSegment>& LinedDepthEnvironment::getParts(){
    validate();
    return parts;
}

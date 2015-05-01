#ifndef SENSOR_ENVIRONMENT_H
#define SENSOR_ENVIRONMENT_H
#include <vector>
#include <lms/imaging/graphics.h>

/**
 * @brief The LinedDepthEnvironment class used to track depths (y-values) on one axis.
 * For example a distance-sensor moved along one axis that is perpendicular to the move-direction
 * TODO only works for positive deltaX-values!
 */
class LinedDepthEnvironment{
public:
    /**
     * @brief The LineDepthSegment struct a segment found in the LinedDepthEnvironment that covers the requirements
     */
    struct LineDepthSegment{
        int startIndex;
        int endIndex;
        float startX;
        float endX;
        float getXDistance(){
            return endX - startX;
        }
        /**
         * @brief y the depth of the LineDepthSegment
         */
        float y;
        LineDepthSegment();
        LineDepthSegment(int startIndex, int endIndex,float startX,float endX, float y):startIndex(startIndex),endIndex(endIndex),startX(startX),endX(endX), y(y){
        }
    };

private:
    float xBuffer;
    std::vector<LineDepthSegment> parts;
    std::vector<float> m_distances;
    float m_stepLength, m_sensivity;
    float m_minY, m_maxY;
public:

    LinedDepthEnvironment(float stepLength, float sensivity);

    bool add(float deltaX,float distance);
    /**
     * @brief draw
     * @param distanceInPixel how many meters one pixel is
     */
    void draw(lms::imaging::Graphics *graphics,float distanceInPixel=1);

    void validate();

    const std::vector<LineDepthSegment>& getParts();
};


#endif /* SENSOR_ENVIRONMENT_H */

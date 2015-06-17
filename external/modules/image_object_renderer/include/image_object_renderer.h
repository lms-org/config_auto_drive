#ifndef IMAGE_HINT_TRANSFORMER_H
#define IMAGE_HINT_TRANSFORMER_H

#include "lms/module.h"
#include "lms/datamanager.h"
#include "lms/imaging_detection/image_hint.h"
#include "street_environment/road.h"
#include "street_environment/obstacle.h"

class ImageObjectRenderer : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:

    std::vector<std::string> environments;
    std::vector<std::string> polylines;
    std::vector<std::string> vertex2f;
    std::vector<std::string> vertex4f;

    lms::imaging::Image *image;
    lms::imaging::BGRAImageGraphics *graphics;
    std::vector<const street_environment::Environment*> toDrawEnv;
    std::vector<const lms::math::polyLine2f*> toDrawPolyLines;
    std::vector<const lms::math::vertex2f*> toDrawVertex2f;
    std::vector<const std::pair<lms::math::vertex2f,lms::math::vertex2f>*> toDrawVertex4f;
    void drawObstacle(const street_environment::Obstacle &obstacle);
    void drawPolyLine(const lms::math::polyLine2f &lane);
    void drawVertex2f(const lms::math::vertex2f &v);
    void drawVertex4f(const std::pair<lms::math::vertex2f,lms::math::vertex2f> &v);

    void setColor(std::string toDrawName);

};

#endif /* IMAGE_HINT_TRANSFORMER_H */

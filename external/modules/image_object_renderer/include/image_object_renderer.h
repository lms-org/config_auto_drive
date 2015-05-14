#ifndef IMAGE_HINT_TRANSFORMER_H
#define IMAGE_HINT_TRANSFORMER_H

#include "lms/module.h"
#include "lms/datamanager.h"
#include "lms/imaging/find/image_hint.h"
#include "image_objects/environment.h"

class ImageObjectRenderer : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    lms::imaging::Image *image;
    std::vector<const Environment*> toDrawEnv;
    std::vector<const lms::math::polyLine2f*> toDrawPolyLines;
    std::vector<const lms::math::vertex2f*> toDrawVertex2f;
    std::vector<const lms::math::vertex<4,float>*> toDrawVertex4f;
    void drawPolyLine(const lms::math::polyLine2f &lane);
    void drawVertex2f(const lms::math::vertex2f &v);
    void drawVertex4f(const lms::math::vertex<4,float> &v);
};

#endif /* IMAGE_HINT_TRANSFORMER_H */

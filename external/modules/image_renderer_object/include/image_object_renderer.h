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
    std::vector<const Environment*> toDraw;
    void drawRoadLane(const Environment::RoadLane &lane);
};

#endif /* IMAGE_HINT_TRANSFORMER_H */

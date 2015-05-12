#ifndef IMAGE_HINT_TRANSFORMER_H
#define IMAGE_HINT_TRANSFORMER_H

#include "lms/module.h"
#include "lms/datamanager.h"
#include "lms/imaging/find/image_hint.h"
#include "image_objects/environment.h"

class ImageObjectMerger : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    void transform(const Environment::RoadLane &fromLane,
                                      Environment::RoadLane &toLane, int sign);

    const Environment *input;
    Environment *output;
};

#endif /* IMAGE_HINT_TRANSFORMER_H */

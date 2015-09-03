#ifndef IMAGE_HINT_FILTER_H
#define IMAGE_HINT_FILTER_H

#include "lms/module.h"
#include "street_environment/road.h"
#include "lms/extra/time.h"

class LinePersistent : public lms::Module {
public:
    struct LaneStore{
        lms::math::polyLine2f lane;
        lms::extra::PrecisionTime created;
        int cycleCreated;
    };

    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    std::vector<LaneStore> lanes;
    const lms::math::polyLine2f *input;
    lms::math::polyLine2f *output;
    void addLine(const lms::math::polyLine2f &line);
    void validateLines();
    void createOutput();
};

#endif /* IMAGE_HINT_FILTER_H */

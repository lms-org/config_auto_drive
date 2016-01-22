#ifndef SOBEL_THRESHOLD_GENERATOR_H
#define SOBEL_THRESHOLD_GENERATOR_H

#include <lms/module.h>
#include "lms/imaging/image.h"
#include "fisher_breaks.h"

/**
 * @brief LMS module sobel_threshold_generator
 **/
class SobelThresholdGenerator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;

private:
    void resetFisherData();
    void fisherHistogram(const uint8_t *imageIn, int nc, int nr, int startRow, int endRow);
    bool pairCompare(const ValueCountPair &firstElem, const ValueCountPair &secondElem);

    lms::ReadDataChannel<lms::imaging::Image> target;
    lms::imaging::Image sobelBuffer;

    std::vector<int> fisherHistData;
    ValueCountPairContainer fisherData;

    int cycleCount;

};

#endif // SOBEL_THRESHOLD_GENERATOR_H

#ifndef SOBEL_THRESHOLD_GENERATOR_H
#define SOBEL_THRESHOLD_GENERATOR_H

#include <lms/datamanager.h>
#include <lms/module.h>
#include "lms/imaging/image.h"

/**
 * @brief LMS module sobel_threshold_generator
 **/
class SobelThresholdGenerator : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;

private:
    lms::ReadDataChannel<lms::imaging::Image> target;
    lms::imaging::Image sobelBuffer;
    //int kernelSobel[9] = {-2, -2, 0, -2, 0, 2, 0, 2, 2};
    void sobelWithoutSqrt(const uint8_t *imageIn, uint8_t *imageOut,  int nc, int nr, int startRow, int endRow);
};

#endif // SOBEL_THRESHOLD_GENERATOR_H

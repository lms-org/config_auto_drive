#include <image_detection/test.h>

extern "C" {
void* getInstance () {
    return new ImageDetectionTest();
}
}

#include "image_hint_merger.h"

extern "C" {

void* getInstance() {
    return new ImageObjectMerger();
}
}

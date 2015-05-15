#include "image_hint_filter.h"

extern "C" {

void* getInstance() {
    return new EnvironmentFilter();
}
}

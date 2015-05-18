#include "environment_line_persistent.h"

extern "C" {

void* getInstance() {
    return new LinePersistent();
}
}

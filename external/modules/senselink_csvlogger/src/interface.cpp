#include "senselink_csvlogger.h"

extern "C" {
void* getInstance () {
    return new senselink_csvlogger();
}
}

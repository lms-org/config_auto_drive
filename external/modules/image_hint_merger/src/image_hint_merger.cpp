#include "image_hint_merger.h"
#include "lms/imaging/find/line.h"
#include "lms/imaging/warp.h"

bool ImageObjectMerger::initialize() {

    input = datamanager()->readChannel<Environment>(this, "ENVIRONMENT_INPUT");
    output = datamanager()->writeChannel<Environment>(this, "ENVIRONMENT_OUTPUT");
    return true;
}

bool ImageObjectMerger::deinitialize() {
    return true;
}

bool ImageObjectMerger::cycle() {

    //TODO just for testing, later on there has to be some config that states what ImageObjects of what ENVIRONMENT should be merged

    return true;
}

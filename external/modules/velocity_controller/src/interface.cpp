#include "velocity_controller.h"

extern "C" {

void* getInstance() {
    return new VelocityController();
}

}

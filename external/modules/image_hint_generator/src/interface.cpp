#include "image_hint_generator.h"

extern "C" {

void* getInstance() {
    return new ImageHintGenerator();
}

}

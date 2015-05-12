#include "image_object_renderer.h"

extern "C" {

void* getInstance() {
    return new ImageObjectRenderer();
}

}

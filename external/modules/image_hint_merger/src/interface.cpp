#include "image_hint_transformer.h"

extern "C" {

void* getInstance() {
    return new ImageHintTransformer();
}

}

#include "socket_datatype_definer.h"

extern "C" {
void* getInstance () {
    return new SocketDatatypeDefiner();
}
}

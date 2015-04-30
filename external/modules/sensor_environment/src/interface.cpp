#include <socket_data_sender/sender.h>

extern "C" {
void* getInstance () {
    return new Sender();
}
}

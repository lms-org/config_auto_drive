#ifndef SOCKET_DATATYPE_DEFINER_H
#define SOCKET_DATATYPE_DEFINER_H

#include <lms/module.h>

/**
 * @brief LMS module socket_datatype_definer, just needed to define some types that are not known
 **/
class SocketDatatypeDefiner : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
};

#endif // SOCKET_DATATYPE_DEFINER_H

#ifndef LMS_DEFINITIONS_H
#define LMS_DEFINITIONS_H

#define LMS_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#define LMS_VERSION_CODE LMS_VERSION(1, 2, 0)
#define LMS_VERSION_STRING "1.2.0"
#define LMS_VERSION_MASK LMS_VERSION(0xff, 0xff, 0)

#define LMS_CONFIGS "/Users/lukas/phoenix_lms/config_auto_drive/configs"
#define LMS_MODULES "/Users/lukas/phoenix_lms/config_auto_drive/build2/external/modules"
#define LMS_SERVICES ""

// Deprecated
#define LMS_EXTERNAL "/Users/lukas/phoenix_lms/config_auto_drive/build2/external"

#endif  // LMS_DEFINITIONS_H

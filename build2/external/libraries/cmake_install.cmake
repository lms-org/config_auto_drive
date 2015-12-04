# Install script for directory: /Users/lukas/phoenix_lms/config_auto_drive/external/libraries

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/googletest_lib/cmake_install.cmake")
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/cereal/cmake_install.cmake")
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/math_lib/cmake_install.cmake")
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/imaging/cmake_install.cmake")
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/imaging_detection/cmake_install.cmake")
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/mavlink-messages-c/cmake_install.cmake")
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/sense_link/cmake_install.cmake")
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/sensor_utils/cmake_install.cmake")
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/socket_connection/cmake_install.cmake")
  include("/Users/lukas/phoenix_lms/config_auto_drive/build2/external/libraries/street_environment/cmake_install.cmake")

endif()


#config_auto_drive
Configuration for testing image-algo, the corresponding environment-and trajectory-creation.

Used for fast prototyping :)

#Requirements
 * Eigen: http://neuro.debian.net/pkgs/libeigen3-dev.html
 * Kalman:  https://github.com/mherb/kalman
 * sfml: http://www.sfml-dev.org/
 
#Crosscompiling for odroid:
 * install arm-linux-gnueabihf
  * apt-get install g++arm-linux-gnueabihf
  * apt-get install gccarm-linux-gnueabihf
 * install ueye drivers in arm-include/link folder
  * /user/arm-linux-gnueabihf/lib
  * /user/arm-linux-gnueabihf/include
 

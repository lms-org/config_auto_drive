#config_auto_drive
Configuration for testing image-algo, the corresponding environment-and trajectory-creation.

Used for fast prototyping :)

#Requirements
 * Eigen: http://neuro.debian.net/pkgs/libeigen3-dev.html
 * Kalman:  https://github.com/mherb/kalman
 * sfml: http://www.sfml-dev.org/


#Start commands
 * Start from file `./lms -c mycam --flags files --enable-load /home/phibedy/CC2016/neue_daten/lmslog/2016-01-25T17-01-22Z-normale_runde`
 * Start car `./lms -c new_car --flags sendData

 
#Crosscompiling for odroid:
 * install arm-linux-gnueabihf
  * apt-get install g++arm-linux-gnueabihf
  * apt-get install gccarm-linux-gnueabihf
 * install ueye drivers in arm-include/link folder
  * /user/arm-linux-gnueabihf/lib
  * /user/arm-linux-gnueabihf/include
 

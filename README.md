#config_auto_drive
Configuration for testing image-algo, the corresponding environment-and trajectory-creation.

Used for fast prototyping :)

#Requirements
 * Eigen: http://neuro.debian.net/pkgs/libeigen3-dev.html
 * Kalman:  https://github.com/mherb/kalman
 * sfml: http://www.sfml-dev.org/


#Start commands
 * Start from file `./lms -c mycam --flags files --enable-load /home/phibedy/CC2016/neue_daten/lmslog/2016-01-25T17-01-22Z-normale_runde`
 * Start car `./lms -c new_car --flags sendData,sdl

 
#Crosscompiling for odroid:
 * install arm-linux-gnueabihf
  * apt-get install g++arm-linux-gnueabihf
  * apt-get install gccarm-linux-gnueabihf
 * install ueye drivers in arm-include/link folder
  * /user/arm-linux-gnueabihf/lib
  * /user/arm-linux-gnueabihf/include
 
#How to install it:
```
pip install conan --upgrade
conan remote add lms http://mineforce.de:9300
git clone https://github.com/tum-phoenix/config_auto_drive
cd config_auto_drive
mkdir build
cd build
conan install .. --build=missing
cmake ..
make


```

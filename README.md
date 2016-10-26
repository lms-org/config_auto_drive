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

//install basics
//automake is needed for protobuffers
sudo apt-get update
sudo apt-get install python python-pip build-essential make git cmake automake
//install conan
sudo pip install conan
//add remote to conan
conan remote add lms http://mineforce.de:9300
//go to your working direktory
cd <your working direktory>
//clone config_auto_drive
git clone https://github.com/tum-phoenix/config_auto_drive
cd config_auto_drive
git submodule init
git submodule update
//build it
mkdir build
cd build
//install conan dependencies and build them if needed
conan install .. --build=missing
//compile it
cmake ..
make -j<number of cores>


```

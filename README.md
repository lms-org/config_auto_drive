# Main Configuration for TUM Phoenix Drive Software 
Configuration which includes all necessary configuration files and includes components as git submodules.

We use LMS (https://github.com/lms-org/lms) as Middleware Framework.

We require some (ubuntu) packages:
- basic build stuff
- automake is needed for protobuffers
- libsdl2 because of mac
- qt because we have some fancy viz stuff going on
- currently only support for specific compiler versions: gcc-4.8 and g++-4.8

Tip: HowTo change compiler in ubuntu: https://askubuntu.com/questions/466651/how-do-i-use-the-latest-gcc-on-ubuntu

Additionally we use Conan (https://conan.io/) to build some more libs (like openCV, ...) from source. This makes sure that we always have the correct version.
 
# How to install
```
sudo apt-get update
sudo apt-get install python python-pip build-essential make git cmake automake libtool libsdl2-dev gcc-4.8 g++-4.8 qt5-default

// install conan
sudo pip install conan

// add remote to conan
conan remote add lms http://mineforce.de:9300

// go to your working direktory
cd <your working direktory>

// clone config_auto_drive
git clone https://github.com/tum-phoenix/config_auto_drive
cd config_auto_drive
git submodule init
git submodule update

// install ximea_driver
sudo ./external/modules/ximea_importer/ximea_driver/install 

// create build directory
mkdir build
cd build

// install conan dependencies and build them if needed
conan install .. --build=missing

// compile it
cmake ..
make -j<number of cores>
```



# How to start it?
Look at the `*.sh` scripts

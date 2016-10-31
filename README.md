#config_auto_drive
Configuration for testing image-algo, the corresponding environment-and trajectory-creation.

Used for fast prototyping :)
 
#How to install it:
```

//install basics
//automake is needed for protobuffers
sudo apt-get update
sudo apt-get install python python-pip build-essential make git cmake automake libtool

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

//create build directory
mkdir build
cd build

//install conan dependencies and build them if needed
conan install .. --build=missing
//compile it
cmake ..
make -j<number of cores>
```
If any error occur, please ask google first, if you are sure that you didn't do a mistake or you can't find a solution to your problem, please write an issue :)


#How to start it?
Look at the `start.sh` script

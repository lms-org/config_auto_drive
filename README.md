# Main Configuration for TUM Phoenix Drive Software 
Configuration which includes all necessary configuration files and includes components as git submodules.

We use LMS (https://github.com/lms-org/lms) as Middleware Framework.

We require some packages:
- basic build stuff
- automake is needed for protobuffers
- libsdl2 because of mac
- qt because we have some fancy viz stuff going on
- currently only support for specific compiler versions: gcc-4.8 and g++-4.8

Tip: HowTo change compiler in ubuntu: https://askubuntu.com/questions/466651/how-do-i-use-the-latest-gcc-on-ubuntu

Additionally we use Conan (https://conan.io/) to build some more libs (like openCV, ...) from source. This makes sure that we always have the correct version. We have our own Conan Server running at http://mineforce.de:9300
 
# Achievements
* First place (BasicCup 2018)[https://wiki.ifr.ing.tu-bs.de/carolocup/carolo-cup-2018] (CaroloCup with old rulebook)
* Third place (Carolocup 2017)[https://wiki.ifr.ing.tu-bs.de/carolocup/carolo-cup-2017]

# How to install
1. install extra ubuntu packages
```
sudo apt-get update
sudo apt-get install python python-pip build-essential make git cmake automake libtool libsdl2-dev gcc-4.8 g++-4.8 qt5-default
```

2. install conan via pip

`sudo pip install conan`

3. add remote to conan

`conan remote add lms http://mineforce.de:9300`

4. go to your working direktory

`cd <your working direktory>`

5. clone config_auto_drive
```
git clone https://github.com/lms-org/config_auto_drive
cd config_auto_drive
```

6. set up submodules
```
git submodule init
git submodule update
```

7. set up extra submodule in trajectory controller (TODO: remove in future)
```
cd external/modules/trajectory_controller/
git submodule init
git submodule update
cd ../../../
```

8. install ximea_driver

`sudo external/modules/ximea_importer/ximea_driver/install` 

9. create build directory
```
mkdir build
cd build
```

10. install conan dependencies (grab a coffee ☕)
preperations: 
* make sure gcc-4.8 and g++-4.8 are installed.
* in ~/.conan/profiles/default change the compiler to gcc and the version to 4.8
* use following command to modify the systems compilers: 
- `sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 100`
- `sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 100`

then you are ready for:

`conan install .. --build=outdated --build=missing`

11. compile it
```
cmake ..
make -j<number of cores>
```
12. add /dev/* symlinks for hokuyo lidar and senseboard and change mode of ximea
```
echo 'ACTION=="add", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="5740", MODE="0666", SYMLINK+="senseboard"' | sudo tee /etc/udev/rules.d/98-senseboard.rules
echo 'KERNEL=="ttyACM[0-9]*", SUBSYSTEM=="tty", SUBSYSTEMS=="usb", ATTRS{idVendor}=="15d1", ATTRS{idProduct}=="0000", MODE="0666", SYMLINK+="hokuyo_lidar"' | sudo tee /etc/udev/rules.d/97-hokuyo_lidar.rules
sudo sed -i -e 's/ATTRS{idProduct}=="3001", GROUP="plugdev"/ATTRS{idProduct}=="3001", MODE="0666", GROUP="plugdev"/g' /etc/udev/rules.d/99-ximea.rules
```


# How to start it?
Look at the `*.sh` scripts

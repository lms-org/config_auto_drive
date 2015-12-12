#!/bin/sh
sudo apt-get update
sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf make git cmake

# Download and install ueye drivers for armhf
wget http://cloud.tum-phoenix.de/f/f155b7f08b/?raw=1 -O /tmp/ueye_armhf.tgz
mkdir /tmp/ueye_armhf
tar -xf /tmp/ueye_armhf.tgz -C /tmp/ueye_armhf

target=/usr/arm-linux-gnueabihf

# Copy includes
sudo cp /tmp/ueye_armhf/usr/include/ueye.h ${target}/include

# Copy libs
sudo cp /tmp/ueye_armhf/usr/lib/libueye_api.so.4.61 ${target}/lib/libueye_api.so

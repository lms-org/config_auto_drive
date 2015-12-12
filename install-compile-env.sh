#!/bin/sh
sudo apt-get update
sudo apt-get install build-essential make git cmake libogre-1.9-dev libois-1.3.0 libois-dev libsfml-dev unzip wget

if [ ! -d "/usr/local/lib/OGRE" ]; then
  if [ -d "/usr/lib/x86_64-linux-gnu/OGRE-1.9.0" ]; then
    echo "Create symlink to /usr/local/lib/OGRE"
    sudo ln -s /usr/lib/x86_64-linux-gnu/OGRE-1.9.0/ /usr/local/lib/OGRE
  fi
fi

# Download and install ueye drivers for amd64
wget http://cloud.tum-phoenix.de/f/00756df192/?raw=1 -O /tmp/ueye_amd64.zip
unzip /tmp/ueye_amd64.zip -d /tmp/ueye_amd64
chmod +x /tmp/ueye_amd64/ueyesdk-setup-4.40-usb-amd64.gz.run
sudo /tmp/ueye_amd64/ueyesdk-setup-4.40-usb-amd64.gz.run

export LD_LIBRARY_PATH="/home/phoenix/dev/config_auto_drive/build/lib"
./build/bin/lms run ./configs/start_car.xml -s -j 2 -l ./build/lib --log error -f start_lidar -f send


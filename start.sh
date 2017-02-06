export LD_LIBRARY_PATH="/home/phoenix/deploy_auto_drive-phibedy/build/lib"
./build/bin/lms run ./configs/start_car.xml -s -j 2 -l ./build/lib --log debug -f start_lidar -f send

export LD_LIBRARY_PATH="/home/phoenix/deploy_auto_drive-phibedy/build/lib"
./build/bin/lms run ./configs/start_car.xml -s -l ./build/lib --log debug --debug -f start_lidar -f send

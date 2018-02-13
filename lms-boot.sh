#!/bin/bash

# Hokuyo
#udev-wait tty 15d1 0000

# XIMEA
#udev-wait usb 20f7 3001

# Senseboard
#udev-wait tty 0483 5740

echo "$(date) start lms-boot" >> /tmp/boot.log

until sudo -u phoenix ximea-test 2>> /tmp/boot.log; do
	echo "$(date) ximea" >> /tmp/boot.log
	sleep 1
done

until sudo -u phoenix hokuyo-test /dev/hokuyo_lidar 2>> /tmp/boot.log; do
	echo "$(date) hokuyo" >> /tmp/boot.log
	sleep 1
done

echo "$(date) start lms" >> /tmp/boot.log

# start LMS
#su phoenix -l -c "cd /home/phoenix/deploy_auto_drive-phibedy && ./start.sh"
cd /home/phoenix/dev/config_auto_drive && ./start.sh > /dev/null

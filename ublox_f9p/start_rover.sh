#!/bin/bash

TS=`date +%Y%m%d-%H%M`
STR2STR=/home/pi/RTKLIB/app/str2str/gcc/str2str

touch rover-started-${TS}.log

rm -f rover-script-starting-* rover-tcm-in-* rover-tcpsrv-* rover-blink-*

BASE=$1
DEVICE=/dev/ttyACM0

if [ "$#" -ne 1 ]; then
	echo "./start_rover.sh <base-station-ip>"
	exit 1
fi

# Read RTCM from base station and send to GNSS receiver
echo "Piping RTCM into rover GNSS receiver"
${STR2STR} -in tcpcli://${BASE}:21101 -out file://${DEVICE} >& rover-rtcm-in-${TS}.log &
sleep 2

# Make roving GNSS data available 
echo "Publishing rover GNSS observations on port 21102"
${STR2STR} -in serial://ttyACM0:230400 -out tcpsvr://:21102 -c ./cfg_f9p_rover.cmd >& rover-tcpsrv-${TS}.log &
sleep 2

# PVT messages only on 21103 (for bandwidth efficiency)
echo "Publishing rover PVT on port 21103"
netcat localhost 21102 |  ../src/ubx/ubx_nav_pvt | ${STR2STR} -out tcpsvr://:21103 >& rover-PVT-out-${TS}.log &

# Channel rover NMEA to Raspberry Pi Sensehat LED blinker
#netcat localhost 21102 | /home/pi/GNSS_RTK/ublox_m8p/sense_hat_indicator >& rover-blink-${TS}.log &

netcat localhost 21102 > rover-${TS}.ubx &


# PVT service
netcat localhost 21102 | ../src/ubx/ubx_nav_pvt | ${STR2STR} -out tcpsvr://:21103


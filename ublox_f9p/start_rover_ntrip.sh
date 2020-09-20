#!/bin/bash

# Start attached F9P as rover using NTRIP server as base station
# start_rover_ntrip.sh  ntripurl lat lng alt
# Supply NTRIP access URL as parameter:
# user:password@host:port/mountpoint       
# Mountpoint should provide RTCM3 messages

DEVICE=/dev/ttyACM0
TS=`date +%Y%m%d-%H%M`
STR2STR=/home/pi/RTKLIB/app/str2str/gcc/str2str

NTRIP_SERVER=$1
LAT=$2
LNG=$3
ALT=$4

touch rover-started-${TS}.log

rm -f rover-script-starting-* rover-tcm-in-* rover-tcpsrv-* rover-blink-*

if [ "$#" -ne 4 ]; then
	echo "./start_rover.sh <ntrip-url> <lat> <lng> <alt>"
	exit 1
fi

echo "Starting NTRIP connection..."
${STR2STR} -in ntripcli://${NTRIP_SERVER}#rtcm3 -p $LAT $LNG $ALT -n 1 -out file://${DEVICE} >& rover-rtcm-in-${TS}.log &

sleep 5

# Make roving GNSS data available 
echo "Publishing rover GNSS observations on port 21102"
#${STR2STR} -in serial://ttyACM0:230400 -out tcpsvr://:21102 -c ./cfg_f9p_rover.cmd >& rover-tcpsrv-${TS}.log &
${STR2STR} -in serial://ttyACM0:230400 -out tcpsvr://:21102 >& rover-tcpsrv-${TS}.log &
sleep 5

# PVT messages only on 21103 (for bandwidth efficiency)
echo "Publishing rover PVT on port 21103"
netcat localhost 21102 |  ../src/ubx/ubx_nav_pvt | ${STR2STR} -out tcpsvr://:21103 >& rover-PVT-out-${TS}.log &

sleep 5

# Channel rover NMEA to Raspberry Pi Sensehat LED blinker
#netcat localhost 21102 | /home/pi/GNSS_RTK/ublox_m8p/sense_hat_indicator >& rover-blink-${TS}.log &

echo "Logging observations to local log file..."
netcat localhost 21102 > rover-${TS}.ubx &

sleep 5

# PVT service
netcat localhost 21102 | ../src/ubx/ubx_nav_pvt | ${STR2STR} -out tcpsvr://:21103

sleep 5

# Gamepad
#../src/util/gamepad_events /dev/input/event0 | ${STR2STR} -out tcpsvr://:21104

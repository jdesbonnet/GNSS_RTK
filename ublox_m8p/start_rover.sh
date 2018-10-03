#!/bin/bash

TS=`date +%Y%m%d-%H%M`
STR2STR=/home/pi/RTKLIB/app/str2str/gcc/str2str


touch rover-started-${TS}.log

rm -f rover-script-starting-* rover-tcm-in-* rover-tcpsrv-* rover-blink-*


# wait for VPN
sleep 10

#${STR2STR} -in serial://ttyACM0:230400#ubx -out /home/pi/rover-${TS}.ubx -c /home/pi/cfg_m8p_raw_nmea.cmd >& /home/pi/rover-${TS}.log

SERVER=10.7.0.53
DEVICE=/dev/ttyACM0

# Read RTCM from base station and send to GNSS receiver
${STR2STR} -in tcpcli://${SERVER}:21101 -out file://${DEVICE} >& rover-rtcm-in-${TS}.log &
sleep 2

# Make roving GNSS data available 
${STR2STR} -in serial://ttyACM0:230400 -out tcpsvr://:21102 -c ./cfg_m8p_rover.cmd >& rover-tcpsrv-${TS}.log &
sleep 2

# Channel rover NMEA to Raspberry Pi Sensehat LED blinker
netcat localhost 21102 | /home/pi/GNSS_RTK/ublox_m8p/sense_hat_indicator >& rover-blink-${TS}.log &

netcat localhost 21102 > rover-raw-${TS}.ubx &

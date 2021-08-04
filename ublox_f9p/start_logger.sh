#!/bin/bash

# Script to log ubx and NMEA from F9P GNSS receiver to file
# Will need utils to separate out UBX from NMEA.

TS=`date +%Y%m%d-%H%M`
STR2STR=/home/pi/RTKLIB/app/str2str/gcc/str2str

DEVICE=$1

if [ "$#" -ne 1 ]; then
	echo "./start_logger.sh <ttyDevice>"
	exit 1
fi

${STR2STR} -in serial://${DEVICE}:230400 -out file://logger-${TS}-${DEVICE}.log -c ./cfg_f9p_rover.cmd 


#!/bin/bash
#
# Make RTCM messages from a GNSS receiver in base mode available to roving stations.
# Usage: ./rtcm_send.sh <port> <device>
#   eg   ./rtcm_send.sh 22101 ttyACM0
# Note that in this case the the /dev prefix is omitted from the device name.

STR2STR=str2str
PORT=$1
DEVICE=$2
${STR2STR} -in serial://${DEVICE}:230400#ubx -out tcpsvr://:${PORT}


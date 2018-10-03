#!/bin/bash
#
# Connect to TCP port with RTCM messages and send to GNSS receiver.
# Usage:  ./rtcm_recv.sh <rtcm-server> <device>
#   eg    ./rtcm_recv.sh localhost:22101 /dev/ttyACM1
#

STR2STR=str2str
SERVER=$1
DEVICE=$2

${STR2STR} -in tcpcli://${SERVER} -out file://${DEVICE} 


#!/bin/bash
# Make rover data available on TCP server. 
#
DEVICE=$1
PORT=$2
str2str -in file://${DEVICE} -out tcpsvr://:${PORT}


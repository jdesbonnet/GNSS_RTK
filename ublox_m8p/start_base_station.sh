#!/bin/bash
STR2STR=str2str
DEVICE=$1
${STR2STR} -in serial://${DEVICE}:230400#ubx -out tcpsvr://:22101 -c ./cfg_m8p_basestation.cmd


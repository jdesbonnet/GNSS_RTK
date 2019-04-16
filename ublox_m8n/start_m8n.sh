#!/bin/bash
#str2str -in serial://ttyUSB0:9600#ubx -out tcpsvr://:21103 -c ./cfg_m8n_raw.cmd
str2str -in serial://ttyUSB0:115200#ubx -out tcpsvr://:21103 -c ./cfg_m8n_raw.cmd

#!/bin/bash
str2str -in serial://ttyUSB0:115200#ubx -out tcpsvr://:21102 -c ./cfg_m8n_raw.cmd

#!/bin/bash
STR2STR=str2str
${STR2STR} -in serial://ttyACM0:230400#ubx -out rover.ubx -c ./cfg_m8p_raw_nonmea.cmd


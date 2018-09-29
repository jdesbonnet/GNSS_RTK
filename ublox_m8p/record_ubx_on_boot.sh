#!/bin/bash
TS=`date +%Y%m%d-%H%M`
STR2STR=/home/pi/RTKLIB/app/str2str/gcc/str2str
ls -l /dev/tty* > script-starting-$TS
sleep 20
${STR2STR} -in serial://ttyACM0:230400#ubx -out /home/pi/rover-${TS}.ubx -c /home/pi/cfg_m8p_raw_nmea.cmd >& /home/pi/rover-${TS}.log

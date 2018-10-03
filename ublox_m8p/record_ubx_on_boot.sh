#!/bin/bash
# Add to /etc/rc.local:
#   su pi -c "/home/pi/record_ubx_on_boot.sh" &
#
STATION=base
TS=`date +%Y%m%d-%H%M`
STR2STR=/home/pi/RTKLIB/app/str2str/gcc/str2str
ls -l /dev/tty* > script-starting-$TS
sleep 20
${STR2STR} -in serial://ttyACM0:230400#ubx -out /home/pi/${STATION}-${TS}.ubx -c /home/pi/cfg_m8p_raw_nmea.cmd >& /home/pi/${STATION}-${TS}.log

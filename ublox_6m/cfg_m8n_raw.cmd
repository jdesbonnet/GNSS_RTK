# 
# str2str configuration command file to enable raw measurements
# on uBlox NEO-M8N (ROM 2.01)
# See https://wiki.openstreetmap.org/wiki/UbloxRAW#U-BLOX-M8
# https://groups.google.com/forum/#!topic/diyrovers/4MsQLgzWjVI
#

# Set sample rate low while configuring receiver
!UBX CFG-RATE 1000 1 1

#
# Turn off all NMEA
#

# Turn off NMEA GGA
!UBX CFG-MSG 240 0 0 0 0 0 0 0

# Turn off NMEA GLL
!UBX CFG-MSG 240 1 0 0 0 0 0 0

# Turn off NMEA GSA
!UBX CFG-MSG 240 2 0 0 0 0 0 0

# Turn off NMEA GSV
!UBX CFG-MSG 240 3 0 0 0 0 0 0

# Turn off NMEA RMC
!UBX CFG-MSG 240 4 0 0 0 0 0 0

# Turn off NMEA VTG
!UBX CFG-MSG 240 5 0 0 0 0 0 0

# Turn off NMEA ZDA
!UBX CFG-MSG 240 8 0 0 0 0 0 0


# 115200 bps on UART1
!HEX B5 62 06 00 14 00 01 00 00 00 D0 08 00 00 00 C2 01 00 07 00 03 00 00 00 00 00 C0 7E


# enable TRK-SFRBX03x0F
!HEX B5 62 06 01 03 00 03 0F 01 1D 67

# enable TRK-MEAS 03x10
!HEX B5 62 06 01 03 00 03 10 01 1E 69

# enable NAV-CLOCK
!HEX B5 62 06 01 03 00 01 22 01 2E 87

# enable NAV-SVINFO
!HEX B5 62 06 01 03 00 01 30 01 3C A3




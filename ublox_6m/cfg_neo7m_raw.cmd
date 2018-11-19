# 
# str2str configuration command file to enable raw measurements
# on uBlox NEO-7M 
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




ublox67glo_uart1_rawtrk.cmd
!UBX CFG-MSG 3 10 0 1 0 0 0 0
!UBX CFG-MSG 3 16 0 1 0 0 0 0
!UBX CFG-MSG 1 32 0 1 0 0 0 0
!UBX CFG-MSG 1 34 0 1 0 0 0 0
@
!UBX CFG-MSG 3 10 0 0 0 0 0 0
!UBX CFG-MSG 3 16 0 0 0 0 0 0
!UBX CFG-MSG 1 32 0 0 0 0 0 0
!UBX CFG-MSG 1 34 0 0 0 0 0 0



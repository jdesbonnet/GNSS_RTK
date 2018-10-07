# 
# str2str configuration command file to enable raw measurements
# and suppress NMEA messages.
#

# Set sample rate low while configuring receiver
!UBX CFG-RATE 1000 1 1


#
# Enable Survey Mode using CFG-TMODE3 (Class/ID 0x62 0x71).
# Ref uBlox 8/8M receiver manual section 32.11.31.1
#

# CFG-TMODE3 surveyTime=128sec acc=16m
#     |CFG-TMODE3|len  |v |r |flags|X or lat   |Y or lng   |Z or alt   |HX|HY|HZ|r |fixedPosAcc|svinMinT   |svinAcc    |                       |Chksum
#!HEX B5 62 06 71 28 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 00 00 71 02 00 00 00 00 00 00 00 00 00 93 A8

# CFG-TMODE3 surveyTime=128sec acc=25m (for testing)
#!HEX B5 62 06 71 28 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 80 00 00 00 90 D0 03 00 00 00 00 00 00 00 00 00 83 87

# CFG-TMODE3: surveyTime=600s, svinAcc=1m
#    |CFG-TMODE3|len  |v |r |flags|X or lat   |Y or lng   |Z or alt   |HX|HY|HZ|r |fixedPosAcc|svinMinT   |svinAcc    |                       |Chksum
!HEX B5 62 06 71 28 00 00 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 58 02 00 00 10 27 00 00 00 00 00 00 00 00 00 00 31 C4


#
# Enable RAW messages
#

# turn on UBX RXM-RAWX messages on USB
!UBX CFG-MSG  2 21 0 0 0 1 0 0

# turn on UBX RXM-SFRBX messages on USB
!UBX CFG-MSG  2 19 0 0 0 1 0 0

# turn on UBX TIM TM2 messages on USB
!UBX CFG-MSG 13 3 0 0 0 1 0 0

# turn on UBX NAV-SVIN (survey in data) on USB
!UBX CFG-MSG  1 59  0 0 0 1 0 0

# turn on UBX NAV-SVINFO on USB
!UBX CFG-MSG  1 48  0 0 0 1 0 0

# turn non UBX NAV-SOL on USB
!UBX CFG-MSG  1  6  0 0 0 1 0 0

#
# Enable RTCM3.2 messages (1005, 1077, 1087, 1230) at 1Hz on USB
#

# Enable RTCM message 1005 (Stationay RTK reference station ARP). Class/ID 0xF5 0x05
!UBX CFG-MSG 245  5  0 0 0 1 0 0 

# Enable RTCM message 1077 (GPS MSM7). Class/ID 0xF5 0x4D
!UBX CFG-MSG 245  77  0 0 0 1 0 0

# Enable RTCM message 1087 (GLONASS MSM7). Class/ID 0xF5 0x57
!UBX CFG-MSG 245  87  0 0 0 1 0 0

# Enable RTCM message 1005 (GLONASS code-phase biases). Class/ID 0xF5 0xE6
!UBX CFG-MSG 245 230  0 0 0 1 0 0





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



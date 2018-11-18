# 
# str2str configuration command file to enable raw measurements
# on uBlox NEO-6M low cost modules.
#

# Set sample rate low while configuring receiver
!UBX CFG-RATE 1000 1 1

#
# Enable RAW messages on u-blox6 (ROM 7.03)
# See https://wiki.openstreetmap.org/wiki/UbloxRAW#U-BLOX6
#
#!HEX b5 62 09 01 10 00 c8 16 00 00 00 00 00 00 97 69 21 00 00 00 02 10 2b 22
#!HEX b5 62 09 01 10 00 0c 19 00 00 00 00 00 00 83 69 21 00 00 00 02 11 5f f0


# From http://xtronix.in/blog/?p=403
!HEX b5 62 06 00 14 00 01 00 00 00 d0 08 00 00 80 25 00 00 07 00 01 00 00 00 00 00 a0 a9 b5 62 06 00 01 00 01 08 22
!HEX b5 62 09 01 10 00 c8 16 00 00 00 00 00 00 97 69 21 00 00 00 02 10 2b 22
!HEX b5 62 09 01 10 00 0c 19 00 00 00 00 00 00 83 69 21 00 00 00 02 11 5f f0
!HEX b5 62 06 01 03 00 02 10 01 1d 66
!HEX b5 62 06 01 03 00 02 11 01 1e 68
!HEX b5 62 09 01 10 00 c8 16 00 00 00 00 00 00 97 69 21 00 00 00 02 10 2b 22
!HEX b5 62 09 01 10 00 0c 19 00 00 00 00 00 00 83 69 21 00 00 00 02 11 5f f0
!HEX b5 62 06 01 03 00 02 10 01 1d 66
!HEX b5 62 06 01 03 00 02 11 01 1e 68

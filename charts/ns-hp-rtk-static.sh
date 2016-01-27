JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin"
NMEA_PATH=$1
${JAVA} NMEAToLatLngAlt < ${NMEA_PATH} | grep DGPS > dgps.dat
${JAVA} NMEAToLatLngAlt < ${NMEA_PATH} | grep NGPS > ngps.dat
${JAVA} NMEAToLatLngAlt < ${NMEA_PATH} | grep FloatRTK > rtk.dat

NMEA_FILE=$(basename "${NMEA_PATH}")
#extension="${filename##*.}"
#NMEA_FILE="${filename%.*}"

BOX_SIZE=10

gnuplot <<- EOF
  set terminal pngcairo size 1600,800
  set output "ns-hp-rtk-test.png"
  set title "NavSpark NS-HP RTK test ${NMEA_FILE}\nBase, rover static and co-located (antennas separated by ~10cm)"

  set grid

  set multiplot layout 1, 2 ;

  set xlabel "Longitude (degrees)"
  set ylabel "Latitude (degrees)"

  plot \
    'ngps.dat' using 3:2 title 'GPS', \
    'dgps.dat' using 3:2 lt 3 title 'DGPS', \
    'rtk.dat' using 3:2 lt 4 title 'Float RTK'


  set xlabel "East/West error (m)"
  set ylabel "North/South error (m)"
  set xrange [-${BOX_SIZE}:${BOX_SIZE}]
  set yrange [-${BOX_SIZE}:${BOX_SIZE}]


  plot \
    'ngps.dat' using 4:5 title 'GPS', \
    'dgps.dat' using 4:5 lt 3 title 'DGPS', \
    'rtk.dat' using 4:5 lt 4 title 'Float RTK'


EOF

#rm dgps.dat ngps.dat rtk.dat


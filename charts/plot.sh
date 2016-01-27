JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin"
${JAVA} NMEAToLatLngAlt < $1 | grep DGPS > dgps.dat
${JAVA} NMEAToLatLngAlt < $1 | grep NGPS > ngps.dat
${JAVA} NMEAToLatLngAlt < $1 | grep FloatRTK > rtk.dat

BOX_SIZE=10

gnuplot <<- EOF
  set terminal pngcairo size 800,800
  set output "ns-hp-rtk-test.png"
  set title "NavSpark NS-HP RTK test\nBase, rover static and co-located (antennas separated by ~10cm)"
  set xlabel "East/West error (m)"
  set ylabel "North/South error (m)"
  set xrange [-${BOX_SIZE}:${BOX_SIZE}]
  set yrange [-${BOX_SIZE}:${BOX_SIZE}]
  set grid
  plot \
    'ngps.dat' using 4:5 title 'GPS', \
    'dgps.dat' using 4:5 lt 3 title 'DGPS', \
    'rtk.dat' using 4:5 lt 4 title 'Float RTK'

EOF

#rm dgps.dat ngps.dat rtk.dat


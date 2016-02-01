#!/bin/bash
JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin"
$JAVA NMEAToSV < $1 > snr.dat

gnuplot <<- EOF
  set terminal pngcairo size 1024,900 background rgb 'black'
  set output "snr.png"

  set border lc rgb 'white'
  set key tc rgb 'white'
  set grid lc rgb 'white'
  set style line 1 linecolor rgb "green"
  set style line 2 linecolor rgb "blue"

  set xlabel "Azimuth (degrees clockwise from north)" textcolor rgb "white"
  set ylabel "Elevation (degrees)" textcolor rgb "white"

  set xrange [0:360]
  set yrange [0:90]

  set palette model RGB defined ( 0 'blue', 50 'red', 80 'yellow', 99 'green' )
  plot 'snr.dat' using 4:3:5 with points palette
EOF


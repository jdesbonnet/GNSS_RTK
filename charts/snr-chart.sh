#!/bin/bash
JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin"
$JAVA NMEAToSV -heatmap heatmap.dat < $1 > snr.dat

gnuplot <<- EOF
  set terminal pngcairo size 1280,720 background rgb 'black'
  set output "snr.png"

  set border lc rgb 'white'
  set key tc rgb 'white'
  set grid lc rgb 'white'
  set style line 1 linecolor rgb "green"
  set style line 2 linecolor rgb "blue"

  set title "GPS satellite signal-to-noise ratio (SNR) as function of position in sky.\nStatic survey with u-Blox Neo-6M module. Limited sky view. Arbitrary SNR units (higher is better)." textcolor rgb "white"

  set label "Joe Desbonnet, 2 Feb 2016" at graph -0.01,-0.05 font "8" tc rgb "white"

  set xlabel "Azimuth (degrees clockwise from north)" textcolor rgb "white"
  set ylabel "Elevation (degrees)" textcolor rgb "white"
  #set y2label "test" textcolor rgb "white"

  set xrange [0:360]
  set yrange [0:90]

  set xtics 45
  set ytics 15
  

  set palette model RGB defined ( 0 'blue', 50 'red', 80 'yellow', 99 'green' )
  plot 'snr.dat' using 4:3:5 with points pointtype 1  palette title ''
EOF

gnuplot <<- EOF

  set terminal pngcairo size 1280,720 background rgb 'black'
  set output "snr-heatmap.png"

  set border lc rgb 'white'
  set key tc rgb 'white'
  set grid lc rgb 'white'
  set style line 1 linecolor rgb "green"
  set style line 2 linecolor rgb "blue"


  set title "GPS satellite signal-to-noise ratio (SNR) as function of position in sky.\nStatic survey with u-Blox Neo-6M module. Limited sky view. Arbitrary SNR units (higher is better)." textcolor rgb "white"


  set label "Joe Desbonnet, 4 Feb 2016" at graph -0.04,-0.08 font "8" tc rgb "white"
  set label "SNR" at graph 1.03,1.05 font "8" tc rgb "white"

  set xlabel "Azimuth (degrees clockwise from north)" textcolor rgb "white"
  set ylabel "Elevation (degrees)" textcolor rgb "white"
  #set y2label "test" textcolor rgb "white"

  set xrange [0:360]
  set yrange [0:90]

  set xtics 45
  set ytics 15

  set palette model RGB defined ( 0 'black', 10 'blue', 50 'red', 80 'yellow', 99 'green' )
  plot 'heatmap.dat' using 1:2:3 with image

EOF



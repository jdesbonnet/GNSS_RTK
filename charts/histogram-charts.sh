#!/bin/bash
JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin:/home/joe/workspace/GNSS_RTK/lib/commons-cli-1.3.1.jar"
NMEA_LOG=$1
if [ ! -e lat5.dat ] ; then
    $JAVA NMEAHistogram -dimension latitude -numsv 5 -binsize 0.000001 < $NMEA_LOG > lat5.dat
fi
if [ ! -e lng5.dat ] ; then
    $JAVA NMEAHistogram -dimension longitude -numsv 5 -binsize 0.000001 < $NMEA_LOG > lng5.dat
fi
if [ ! -e alt5.dat ] ; then
    $JAVA NMEAHistogram -dimension altitude -numsv 5 -binsize 0.1 < $NMEA_LOG > alt5.dat
fi


gnuplot <<- EOF
  set terminal pngcairo size 1280,720 background rgb 'black'


  set border lc rgb 'white'
  set key tc rgb 'white'
  set grid lc rgb 'white'
  set style line 1 linecolor rgb "green"
  set style line 2 linecolor rgb "blue"


  set title "Static survey histogram.\nu-blox NEO-6M receiver. Limited sky view." textcolor rgb 'white'
  set grid

  set style fill solid 1.0

  ##set style arrow 1 head filled size char 1.5,20,50 lc rgb "red"

  cx = -8.9825
  cy = 53.2826



  set ylabel "Count" textcolor rgb 'white'

  set output "histogram-lat.png"
  #set xrange [53.2820:53.2835]
  set xlabel "Latitude (degrees)" textcolor rgb 'white'
  plot \
  'lat5.dat' using 1:2 title '>=5 satellite fix' with boxes lc rgb "#c00000" 

  set output "histogram-lng.png"
  set xlabel "Longitude (degrees)" textcolor rgb 'white'
  plot \
  'lng5.dat' using 1:2 title '>=5 satellite fix' with boxes lc rgb "#c000

  set output "histogram-alt.png"
  set xlabel "Altitude (meters)" textcolor rgb 'white'
  plot \
  'alt5.dat' using 1:2 title '>=5 satellite fix' with boxes lc rgb "#c000


EOF

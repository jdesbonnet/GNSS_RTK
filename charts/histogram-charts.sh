#!/bin/bash
JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin:/home/joe/workspace/GNSS_RTK/lib/commons-cli-1.3.1.jar"
NMEA_LOG=$1
if [ ! -e lat5.dat ] ; then
    $JAVA NMEAHistogram -dimension latitude -numsv 5 -binsize 0.00001 < $NMEA_LOG > lat5.dat
fi
if [ ! -e lng5.dat ] ; then
    $JAVA NMEAHistogram -dimension longitude -numsv 5 -binsize 0.00001 < $NMEA_LOG > lng5.dat
fi
if [ ! -e alt5.dat ] ; then
    $JAVA NMEAHistogram -dimension altitude -numsv 5 -binsize 0.25 < $NMEA_LOG > alt5.dat
fi


gnuplot <<- EOF
  set terminal pngcairo size 1280,720 background rgb 'black'
  set output "histograms.png"

  set border lc rgb 'white'
  set key tc rgb 'white'
  set grid lc rgb 'white'
  set style line 1 linecolor rgb "green"
  set style line 2 linecolor rgb "blue"

  set xrange[-8.985:-8.980]
  set yrange[53.2815:53.2835]

  set title "Effect on accuracy of #satellites in GPS solution.\nFixed location, 8 hours of data. u-blox NEO-6M receiver. Limited sky view." textcolor rgb 'white'
  set xlabel "Longitude (degrees)" textcolor rgb 'white'
  set ylabel "Latitude (degrees)" textcolor rgb 'white'
  set grid



  ##set style arrow 1 head filled size char 1.5,20,50 lc rgb "red"

  cx = -8.9825
  cy = 53.2826

  plot \
  'lat5.dat' using 1:2 title '4 satellite fix' with boxes lc rgb "#c00000" 


EOF

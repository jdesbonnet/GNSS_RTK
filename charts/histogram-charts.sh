#!/bin/bash
JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin:/home/joe/workspace/GNSS_RTK/lib/commons-cli-1.3.1.jar"
NMEA_LOG=$1
if [ ! -e lat5.dat ] ; then
    $JAVA NMEAHistogram -dimension latitude -numsv 5 -binsize 0.000001 < $NMEA_LOG > lat5.dat
fi
if [ ! -e lng5.dat ] ; then
    $JAVA NMEAHistogram -dimension longitude -numsv 5 -binsize 0.000002 < $NMEA_LOG > lng5.dat
fi
if [ ! -e alt5.dat ] ; then
    $JAVA NMEAHistogram -dimension altitude -numsv 5 -binsize 0.1 < $NMEA_LOG > alt5.dat
fi


gnuplot <<- EOF

  # Radius of earth at equator
  R = 6371e3
  meters_per_degree = 3.1415926 * R / 180
  latitude_to_meters(lat,center)=(lat-center)*meters_per_degree;
  lng_to_meters(lng,center,lat)=(lng-center)*meters_per_degree;



  set terminal pngcairo size 1280,720 background rgb 'black'

  set output "static-survey.png"

  set border lc rgb 'white'
  set key tc rgb 'white'
  set grid lc rgb 'white'
  set style line 1 linecolor rgb "green"
  set style line 2 linecolor rgb "blue"

  set multiplot layout 2,3 title "NMEA log analysis, static survey" 

  # Add meter scale to lat/lng. Propose to set 0m as the mean/median.
  # http://sourceforge.net/p/gnuplot/mailman/message/26460144/

  set grid
  set style fill solid 1.0
  #set ylabel "Count" textcolor rgb 'white'
  set xtics font ",6"
  set x2tics font ",6"

  # Generate statistical summary of datasets
  stats "lat5.dat" using 1:2 prefix "LAT" nooutput
  stats "lng5.dat" using 1:2 prefix "LNG" nooutput
  stats "alt5.dat" using 1:2 prefix "ALT" nooutput

  set title "Latitude histogram" textcolor rgb 'white'
  set xlabel "Latitude (degrees)" textcolor rgb 'white'
  set x2label "North/South meters" textcolor rgb 'white'
  set xrange [LAT_min_x:LAT_max_x]
  set x2range [latitude_to_meters(LAT_min_x,LAT_pos_max_y):latitude_to_meters(LAT_max_x,LAT_pos_max_y)]
  set xtics nomirror
  set x2tics
  unset key
  plot 'lat5.dat' using 1:2 title '>=5 satellite fix' with boxes lc rgb "#c00000" 

  set title "Longitude histogram" textcolor rgb 'white'
  set xlabel "Longitude (degrees)" textcolor rgb 'white'
  set x2label "North/South meters" textcolor rgb 'white'
  set xrange [LNG_min_x:LNG_max_x]
  set x2range [lng_to_meters(LNG_min_x,LNG_pos_max_y,53.28):lng_to_meters(LNG_max_x,LNG_pos_max_y,53.28)]
  set xtics nomirror
  set x2tics
  unset key
  plot 'lng5.dat' using 1:2 title '>=5 satellite fix' with boxes lc rgb "#c00000"

  set title "Altitude histogram" textcolor rgb 'white'
  set xlabel "Altitude (meters)" textcolor rgb 'white'
  set xrange [ALT_min_x:ALT_max_x]
  set x2label "Elevation meters" 
  unset key
  plot 'alt5.dat' using 1:2 title '>=5 satellite fix' with boxes lc rgb "#c00000"


EOF

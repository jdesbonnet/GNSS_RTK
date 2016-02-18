#!/bin/bash
JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin:/home/joe/workspace/GNSS_RTK/lib/commons-cli-1.3.1.jar"
NMEA_LOG=$1

dimensions="latitude longitude altitude"
numberOfSv="5 7 9"

for dim in $dimensions; do
  for nsv in $numberOfSv; do
    file="${dim}${nsv}.dat"
    if [ ! -e $file ] ; then
        if [ $dim = "altitude" ] ; then
            binSize="0.1"
        else
            binSize="0.000001"
        fi
        echo "Writing file $file, using bin size $binSize"
        $JAVA NMEAHistogram -dimension $dim -numsv $nsv -binsize $binSize < $NMEA_LOG > $file
    fi
  done
done

if [ ! -e latlngheatmap.dat ]; then
  $JAVA NMEAToLatLngHeatmap < $NMEA_LOG > latlngheatmap.dat
fi

gnuplot <<- EOF

  # Radius of earth at equator
  R = 6371e3
  lat_to_meters(lat,center)=(lat-center)*pi*R/180;
  lng_to_meters(lng,center,lat)=(lng-center)*pi*R*cos(lat*pi/180);


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
  set ytics font ",6"
  set y2tics font ",6"

  # Generate statistical summary of datasets
  stats "latitude5.dat" using 1:2 prefix "LAT" nooutput
  stats "longitude5.dat" using 1:2 prefix "LNG" nooutput
  stats "altitude5.dat" using 1:2 prefix "ALT" nooutput


  #
  # Latitude histogram chart
  #
  set title "Latitude histogram" textcolor rgb 'white'
  set xlabel "Latitude (bottom: degrees, top: meters)" textcolor rgb 'white'
  set xrange [LAT_min_x:LAT_max_x]
  set x2range [lat_to_meters(LAT_min_x,LAT_pos_max_y):lat_to_meters(LAT_max_x,LAT_pos_max_y)]
  set xtics nomirror
  set x2tics
  unset key
  plot 'latitude5.dat' using 1:2 title '>=5 satellite fix' with boxes lc rgb "#c00000", \
       'latitude7.dat' using 1:2 title '>=7 satellite fix' with boxes lc rgb "forest-green", \
       'latitude9.dat' using 1:2 title '>=9 satellite fix' with boxes lc rgb "#400080"

  #
  # Longitude histogram chart
  #
  set title "Longitude histogram" textcolor rgb 'white'
  set xlabel "Longitude (bottom: degrees, top: meters)" textcolor rgb 'white'
  set xrange [LNG_min_x:LNG_max_x]
  set x2range [lng_to_meters(LNG_min_x,LNG_pos_max_y,53.28):lng_to_meters(LNG_max_x,LNG_pos_max_y,53.28)]
  set xtics nomirror
  set x2tics
  unset key
  plot 'longitude5.dat' using 1:2 title '>=5 satellite fix' with boxes lc rgb "#c00000", \
       'longitude7.dat' using 1:2 title '>=7 satellite fix' with boxes lc rgb "forest-green", \
       'longitude9.dat' using 1:2 title '>=9 satellite fix' with boxes lc rgb "#400080"

  #
  # Altitude histogram chart
  #
  set title "Altitude histogram" textcolor rgb 'white'
  set xlabel "Altitude (meters)" textcolor rgb 'white'
  set xrange [ALT_min_x:ALT_max_x]
  set x2range [ALT_min_x:ALT_max_x]
  unset key
  plot 'altitude5.dat' using 1:2 title '>=5 satellite fix' with boxes lc rgb "#c00000", \
       'altitude7.dat' using 1:2 title '>=7 satellite fix' with boxes lc rgb "forest-green", \
       'altitude9.dat' using 1:2 title '>=9 satellite fix' with boxes lc rgb "#400080"


  #
  # Latitude, longitude heatmap
  #
  stats "latlngheatmap.dat" using 1:3 prefix "HM" nooutput

  set title "Latitude, longitude heatmap"
  set xlabel "Longitude (bottom: degrees, top: meters)
  set ylabel "Latitude"

  set xrange [LNG_min_x:LNG_max_x]
  set x2range [lng_to_meters(LNG_min_x,LNG_pos_max_y):lng_to_meters(LNG_max_x,LNG_pos_max_y)]
  set xtics nomirror
  set x2tics

  set yrange [LAT_min_x:LAT_max_x]
  set y2range [lat_to_meters(LAT_min_x,LAT_pos_max_y):lat_to_meters(LAT_max_x,LAT_pos_max_y)]
  set ytics nomirror
  set y2tics	

  set palette model RGB defined ( 0 'black', 10 'blue', 50 'red', 80 'yellow', 99 'white' )

  unset key
  plot 'latlngheatmap.dat' using 2:1:3 with image




  #
  # TODO: show sd, median, mean, peak
  #

EOF

# Experiment to explore how number of satellites affects accurcy.
# Log file on workshop windowsill (indoors) left overnight.
JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin"
if [ ! -e survey.dat ] ; then
${JAVA} NMEAToLatLngAlt < $1 | grep NGPS > survey.dat
grep "NGPS 4" survey.dat > t.4
grep "NGPS 5" survey.dat > t.5
grep "NGPS 6" survey.dat > t.6
grep "NGPS 7" survey.dat > t.7
grep "NGPS 8" survey.dat > t.8
fi
gnuplot <<- EOF
  set terminal pngcairo size 1280,720 background rgb 'black'
  set output "survey.png"

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

  cx = -8.9824
  cy = 53.2824

  set style arrow 8 head size screen 0.008,90 lw 2 lc rgb "white" front

  set arrow from cx,cy to cx+1.5e-4,cy lc rgb "white" lw 2 front
  set arrow from cx,cy to cx+1.5e-4,cy lc rgb "white" as 8
  set arrow from cx,cy to cx,cy+9e-5 lc rgb "white" lw 2 front
  set arrow from cx,cy to cx,cy+9e-5 lc rgb "white" as 8
  set label "10m" at cx,cy offset 0.2,0.5 left textcolor rgb "white" front

  set label "Joe Desbonnet, 24 Jan 2016" at graph -0.1,-0.06 font ",8" tc rgb "white"
  ##set label "http://jdesbonnet.blogspot.com" at graph -0.05,-0.10 font ",8" tc rgb "white"


  plot \
  't.4' using 3:2 title '4 satellite fix' lc rgb "#c00000"  , \
  't.5' using 3:2 title '5 satellite fix' lc rgb "yellow", \
  't.6' using 3:2 title '6 satellite fix' lc rgb "forest-green", \
  't.7' using 3:2 title '7 satellite fix' lc rgb "blue", \
  't.8' using 3:2 title '8 satellite fix' lc rgb "#b000b0"


EOF

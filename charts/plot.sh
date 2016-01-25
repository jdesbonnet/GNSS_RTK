JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin"
${JAVA} NMEAToLatLngAlt < $1 | grep DGPS > dgps.dat
${JAVA} NMEAToLatLngAlt < $1 | grep NGPS > ngps.dat
${JAVA} NMEAToLatLngAlt < $1 | grep FloatRTK > rtk.dat

gnuplot <<- EOF
  set terminal pngcairo size 800,800
  set output "dgps.png"
  set title "NavSpark NS-HP DGPS test\nSuboptimal conditions: limited view of sky, only 4 satellites."
  set xlabel "East/West error (m)"
  set ylabel "North/South error (m)"
  set xrange [-5:5]
  set yrange [-5:5]
  set grid
  plot 'ngps.dat' using 4:5 title 'GPS', 'dgps.dat' using 4:5 lt 3 title 'DGPS', 'rtk.dat' using 4:5 lt 4 title 'Float RTK'

EOF

JAVA="java -cp /home/joe/workspace/FarmEye/WEB-INF/classes"
${JAVA} farmeye.nmea.NMEAToLatLngAlt < $1 | grep DGPS > dgps.dat
${JAVA} farmeye.nmea.NMEAToLatLngAlt < $1 | grep NGPS > ngps.dat
gnuplot <<- EOF
  set terminal pngcairo size 800,800
  set output "dgps.png"
  set title "NavSpark NS-HP DGPS test\nSuboptimal conditions: limited view of sky, only 4 satellites."
  set xlabel "East/West error (m)"
  set ylabel "North/South error (m)"
  set xrange [-1:1]
  set yrange [-1:1]
  set grid
  plot 'dgps.dat' using 4:5 title 'DGPS', 'ngps.dat' using 4:5 lt 3 title 'GPS'

EOF

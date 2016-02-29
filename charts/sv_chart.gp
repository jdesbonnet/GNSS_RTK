set polar
set angles degree

# Azimuth from 0 (N) clockwise to 360 degrees
set trange[0:360]

# Elevation (aka altitude) from 0 - 90, but 90 is at center
set rrange[0:90] 

# Spokes on polar chart at 45 degree increments
set grid polar 45 linecolor rgb 'white'

set tics textcolor rgb 'white'

unset xtics 
unset ytics

# Rings on polar chart (elevation) every 30 degrees
set rtics axis 30 nomirror rotate by 0 offset 0, 0.7  textcolor rgb 'white'

#"remove" the tics so that only the y tics are displayed
#set xtics scale 0 
#set the xtics only go from 0 to 6000 with increment of1000 but do not display anything. 
# This has to be done otherwise the grid will not be displayed correctly.
#set xtics ("" 10, "" 20, "" 30, "" 40, "" 50, "" 60) 
#set ytics 0, 10, 90 #make the ytics go from the center (0) to 6000 with incrment of 1000

# 4 points of compass
set_label(x, text) = sprintf("set label '%s' at (95*cos(%f)), (95*sin(%f)) center textcolor rgb 'white'", text, 90-x, 90-x) 
eval set_label(0, "N")
eval set_label(90, "E")
eval set_label(180, "S")
eval set_label(270, "W")


# Problem with the tic color: border and tic marks are the same color. So can't 
# have no border and have white ticks. But can overwrite border with 'arrows'.
set border linecolor rgb 'white'
#unset border
set arrow from graph 0,graph 1 to graph 1,graph 1 nohead ls -1 lc rgb "black" front
set arrow from graph 1,graph 1 to graph 1,graph 0 nohead ls -1 lc rgb "black" front
set arrow from graph 1,graph 0 to graph 0,graph 0 nohead ls -1 lc rgb "black" front
set arrow from graph 0,graph 0 to graph 0,graph 1 nohead ls -1 lc rgb "black" front


set key textcolor rgb 'blue'

#set terminal pngcairo size 640,640 background rgb 'black'
set terminal pngcairo size 640,640 transparent

set size square

# for SV SNR color circles
set palette model RGB defined ( 0 'blue', 50 'red', 80 'yellow', 99 'green' )
set style fill transparent solid 0.8 noborder
set cbrange [0:45]
set cblabel "Signal Strength" textcolor rgb 'white'

set tmargin at screen 1.2
set bmargin at screen 0.05


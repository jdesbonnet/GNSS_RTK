set polar
set angles degree
set trange[0:360] # radius
set rrange[0:90] 
#set xrange[-90:90]
#set yrange[-90:90]

set grid polar 45 linecolor rgb 'blue'

unset xtics 
unset ytics
#set xtics axis 10
#set ytics axis 30

set rtics axis 30 nomirror rotate by -270  offset 0, 0.7


set xtics scale 0 #"remove" the tics so that only the y tics are displayed
set xtics ("" 10, "" 20, "" 30, "" 40, "" 50, "" 60) #set the xtics only go from 0 to 6000 with increment of1000 but do not display anything. This has to be done otherwise the grid will not be displayed correctly.
set ytics 0, 10, 90 #make the ytics go from the center (0) to 6000 with incrment of 1000

#this places a label on the outside
set_label(x, text) = sprintf("set label '%s' at (95*cos(%f)), (95*sin(%f)) center", text, 90-x, 90-x) 
eval set_label(0, "N")
eval set_label(90, "E")
eval set_label(180, "S")
eval set_label(270, "W")







#set border lc rgb 'white'
unset border

set key textcolor rgb 'blue'

#set terminal pngcairo size 640,640 background rgb 'black'
set terminal pngcairo size 640,640 

set size square

# for SV SNR color circles
set palette model RGB defined ( 0 'blue', 50 'red', 80 'yellow', 99 'green' )
set style fill transparent solid 0.8 noborder
set cbrange [0:45]
set cblabel "SNR"


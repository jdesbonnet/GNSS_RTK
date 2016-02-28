set polar
set angles degree
set trange[0:360] # radius
set rrange[0:90] 
#set xrange[-90:90]
#set yrange[-90:90]

set grid polar 45 lc rgb 'blue'

set xtics axis #disply the xtics on the axis instead of on the border
set ytics axis

#set border lc rgb 'white'
unset border

set key tc rgb 'blue'

#set terminal pngcairo size 640,640 background rgb 'black'
set terminal pngcairo size 640,640 

set size square

# for SV SNR color circles
set style fill transparent solid 0.8 noborder


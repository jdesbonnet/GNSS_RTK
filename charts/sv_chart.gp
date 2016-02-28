set polar
set angles degree
r = 6
set urange[0:r] # radius
set vrange[0:360] # angle

set grid polar 60 #set the grid to be displayed every 60 degrees
set grid ls 10

set border lc rgb 'white'
set key tc rgb 'white'


set terminal pngcairo size 320,320 background rgb 'black'


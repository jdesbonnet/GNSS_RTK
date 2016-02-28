rm f0*.gp
rm f0*.png
rm f0*.dat
FRAMELEN=300
java -cp ../lib/commons-cli-1.3.1.jar:../bin NMEAMovie  \
-lat0 53.2824 -lat1 53.2828 -lng0 -8.983 -lng1 -8.982 \
-latbinsize 0.00001 -lngbinsize 0.00001 -framelength $FRAMELEN < ~/t.t
find . -name "f0*.gp" -exec  gnuplot {} \;
ffmpeg -i f%06d.png -c:v libx264 -r 30 -pix_fmt yuv420p out.mp4


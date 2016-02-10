#!/bin/bash
JAVA="java -cp /home/joe/workspace/GNSS_RTK/bin:/home/joe/workspace/GNSS_RTK/lib/commons-cli-1.3.1.jar"
if [ ! -e lat5.dat ] ; then
    $JAVA NMEAHistogram -dimension latitude -numsv 5 -binsize 0.00001 < ~/grove.log > lat5.dat
fi
if [ ! -e lng5.dat ] ; then
    $JAVA NMEAHistogram -dimension longitude -numsv 5 -binsize 0.00001 < ~/grove.log > lng5.dat
fi
if [ ! -e alt5.dat ] ; then
    $JAVA NMEAHistogram -dimension altitude -numsv 5 -binsize 0.25 < ~/grove.log > alt5.dat
fi




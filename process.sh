#/bin/bash

file=$1

ctime=`grep creation $file | awk '{sum += $2}; END {print sum/NR}'`
utime=`grep update $file | awk '{sum += $2}; END {print sum/NR}'`
rtime=`grep render $file | awk '{sum += $2}; END {print sum/NR}'`

echo "creation:   ${ctime}us"
echo "update:     ${utime}us"
echo "render:     ${rtime}us"

#! /bin/bash
#nsd_hw1

two=2 
count=0 
nums=`expr $# + 1`
if [ $nums -lt 2 ]; then
    echo "missing file name"
elif [ $nums -gt 2 ]; then 
    echo "only one argument is allowed"
else
    filename=$1
    if [ -f $1 ]; then 
        while read line 
        do
            count=`expr $count + 1`
        done < $filename
        echo "$count lines in $filename"
    else 
        echo "$filename not found"
    fi
fi
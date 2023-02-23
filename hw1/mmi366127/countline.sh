#!/bin/bash

if  [ $# -lt 1 ]; then
    echo "missing file name"
elif [ $# -gt 1 ]; then
    echo "only one argument is allowed"
else
    filename=$1
    if test -f "$filename"; then 
        ans=`awk 'END {print NR}' $filename`
        echo "${ans} lines in ${filename}"
    else 
        echo "${filename} not found"
    fi
fi

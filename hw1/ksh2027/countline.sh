#!/bin/bash

fname=$1

if [ $# -lt 1 ] ; then
    echo "missing file name"
elif [ $# -gt 1 ] ; then
    echo "only one argument is allowed"
else
    if [ -e $fname ]; then
        lines=$(cat $fname | wc -l)
        echo "$lines lines in $fname"
    else
        echo "$fname not found"
    fi
fi
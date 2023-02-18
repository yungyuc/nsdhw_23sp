#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "missing file name"
elif [ $# -gt 1 ] ; then
    echo "only one argument is allowed"
else 
    fname=$1
    if [ -f $fname ]; then
        lines="$(grep -c ^ $1)"
        echo "$lines lines in $fname"
    else
        echo "$fname not found"
    fi
fi

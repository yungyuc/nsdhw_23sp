#!/bin/bash

if [ $# -le 0 ]; then
    echo "missing file name"
elif [ $# -gt 2 ]; then
    echo "only one argument is allowed"
else
    if [ -f $1 ]; then
        echo "$(cat $1 | wc -l) lines in $1" 
    else 
        echo "$1 not found"
    fi
fi
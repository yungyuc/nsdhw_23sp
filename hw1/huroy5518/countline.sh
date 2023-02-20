#!/bin/bash


if [ "$#" -lt 1 ]; then
    echo "missing file name"
    exit 0
fi

if [ "$#" -gt 1 ]; then
    echo "only one argument is allowed"
    exit 0
fi

if [ ! -f "$1" ]; then
    echo "$1 not found"
    exit 255
fi

line_number=$(wc -l $1 | tr -dc '0-9')
echo "$line_number lines in $1"

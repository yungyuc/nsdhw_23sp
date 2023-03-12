#!/bin/bash

if [[ $# -lt 1 ]]; then
    printf "missing file name\n"
elif [[ $# -gt 1 ]]; then
    printf "only one argument is allowed\n"
else
    fileName=$1

    if [[ -e $fileName ]]; then
        printf "%s lines in $fileName\n" "$(wc -l $fileName | tr -dc '0-9')"
    else
        printf "$fileName not found\n"
    fi
fi
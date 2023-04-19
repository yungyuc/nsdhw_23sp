#!/bin/bash

if [ "$#" -lt 1 ]; then # lt = less than
  echo "missing file name"
elif [ "$#" -gt 1 ]; then # gt = greater than
  echo "only one argument is allowed"
else
  if [ -f "$1" ]; then # $1 = the first parameter of .sh
      num_lines=$( cat "$1" | wc -l) # wc -l: count lines
      echo "${num_lines} lines in $1"
  else
      echo "$1 not found"
  fi
fi
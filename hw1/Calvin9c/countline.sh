#!/bin/bash

if [ "$#" -lt 1 ]; then
  echo "missing file name"
elif [ "$#" -gt 1 ]; then
  echo "only one argument is allowed"
else
  if [ -f "$1" ]; then
      num_lines=$( cat "$1" | wc -l)
      echo "${num_lines} lines in $1"
  else
      echo "$1 not found"
  fi
fi
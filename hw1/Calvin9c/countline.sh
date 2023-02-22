# !/bin/bash

# $# denote the number of parameter
if [ $# -lt 1 ]; then
  echo "missing file name\n"
elif [ $# -gt 1 ]; then
  echo "only one argument is allowed\n"
else
  fname=$1
  if [ -e fname]; then # -e fname will return true if file exist
    num_lines=$(wc -l < $fname) # wc will return three numbers which stand for the number of line, word and byte respectively. 
    echo "$num_lines lines in $fname\n"
  else
    echo "$fname not found\n"
  fi
fi

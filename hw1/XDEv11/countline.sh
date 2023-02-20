#!/bin/bash

if [ $# -eq 0 ]
then
	echo 'missing file name';
elif [ $# -gt 1 ]
then
	echo 'only one argument is allowed';
else
	wc -l $1;
fi

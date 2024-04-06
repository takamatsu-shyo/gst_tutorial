#! /bin/bash

if [ -z "$1" ]; then
	echo "Please set a tutorial unmber like;  ./compile 01"
	exit
fi

inputString="$1"

if [  ${#inputString} -lt 2 ]; then
	echo "Please set a tutorial number with two digits like;  ./compile 01"
	exit
fi

gcc src/$1.c -o bin/$1 `pkg-config --cflags --libs gstreamer-1.0`

echo "src/$1.c is compiled"
echo "bin/$1 is running"
./bin/$1

#!/bin/sh

# checks number of arguments
if [ "$#" -ne 2 ]; then
  echo "Usage:"
  echo "$0 <time> <fifoname>"
  echo "time - Number of seconds that the server will run"
  echo "fifoname - Name of the fifo to be created"
  exit 1;
fi

# executes server
cd server
./Q1 -t "$1" "$2"

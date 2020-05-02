#!/bin/sh

# checks number of arguments
if [ "$#" -ne 2 ]; then
  echo "Usage:"
  echo "$0 <time> <fifoname>"
  echo "time - Number of seconds that the client will run"
  echo "fifoname - Name of the fifo to connect"
  exit 1;
fi

# executes server
cd client
./U1 -t "$1" "$2"

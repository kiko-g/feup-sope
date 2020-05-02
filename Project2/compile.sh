#!/bin/sh

cd client
make clean && make 2> /dev/null
echo "Compiled client successfuly\n"

cd ../server
make clean && make 2> /dev/null
echo "Compiled server successfuly"
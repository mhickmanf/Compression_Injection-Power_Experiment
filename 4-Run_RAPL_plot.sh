#!/bin/bash

open -a Terminal /users/mlhickm/rapl_plot
make
sudo ./rapl_plot

# Launch script in background
./my_script.sh &
# Get its PID so we know how to kill this process
PID=$!
# Wait for 2 seconds
sleep 2
# Here is where we will want to have a while loop
# That waits until SZ is done compressing/decompressing

# Kill this process
kill $PID
#!/bin/bash

# Get its PID so we know how to kill this process
#PID=$!

cd /users/mlhickm/Compression_Power_Experiment/rapl_plot
make
python3 4-Subprocess.py


#xterm -e ls

#open -a Terminal /users/mlhickm/Compression_Power_Experiment/rapl_plot
#make
#sudo ./rapl_plot

# Launch script in background
#./my_script.sh &
#cmd="pwd"
#nohup $cmd &

# Wait for 2 seconds
#sleep 5
#pwd
#pwd
#pwd
#sleep 5
# Here is where we will want to have a while loop
# That waits until SZ is done compressing/decompressing

# Kill this process
#kill $PID

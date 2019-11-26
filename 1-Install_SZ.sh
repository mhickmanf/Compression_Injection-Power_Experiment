#!/bin/bash

cd /users/mlhickm/Compression_Injection-Power_Experiment/SZ_Files
pwd
wait
pwd
git clone https://github.com/disheng222/SZ.git
wait
cd /users/mlhickm/Compression_Injection-Power_Experiment/SZ_Files/SZ
wait
./configure --prefix=/users/mlhickm/Compression_Injection-Power_Experiment/SZ_Files
wait
make clean
make
make install

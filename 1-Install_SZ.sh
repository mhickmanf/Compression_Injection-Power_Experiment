#!/bin/bash

cd $SZ_Files
git clone https://github.com/disheng222/SZ.git
cd $SZ
./configure --prefix=/users/mlhickm/Compression_Power_Experiment/SZ_Files
make clean
make
make install
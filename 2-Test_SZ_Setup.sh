#!/bin/bash


#cd /users/mlhickm/Compression_Injection-Power_Experiment/data
#wget https://97235036-3749-11e7-bcdc-22000b9a448b.e.globus.org/ds131.2/Data-Reduction-Repo/raw-data/EXAALT/SDRBENCH-EXAALT-2869440.tar.gz
#tar -xvf SDRBENCH-EXAALT-2869440.tar.gz
#wait
cd /users/mlhickm/Compression_Injection-Power_Experiment/
make
wait
./comp_profile data/SZ_test_data/testfloat_8_8_128.dat 0,0,0,0,8192 config/sz.config	


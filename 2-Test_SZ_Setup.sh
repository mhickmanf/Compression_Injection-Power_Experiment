#!/bin/bash


#cd /users/mlhickm/Compression_Power_Experiment/data
#wget https://97235036-3749-11e7-bcdc-22000b9a448b.e.globus.org/ds131.2/Data-Reduction-Repo/raw-data/EXASKY/HACC/EXASKY-HACC-data-medium-size.tar.gz
#tar -xvf EXASKY-HACC-data-medium-size.tar.gz
#wait
cd /users/mlhickm/Compression_Power_Experiment/
make
wait
./comp_profile data/SZ_test_data/testfloat_8_8_128.dat 0,0,0,0,8192 config/sz.config	


#./comp_inj ./data/hacc_data/xx.f32 0,0,0,0,280953867 ./config/sz_psnr_60.config 30 0

#./comp_inj ./data/hacc_data/xx.f32 0,0,0,0,280953867 ./config/sz_psnr_60.config byte_loc 	     flip_loc
#										(0 to compress size) (0 to 7)

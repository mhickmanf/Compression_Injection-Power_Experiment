#!/bin/bash


#cd /users/mlhickm/Compression_Injection-Power_Experiment/data
#wget https://97235036-3749-11e7-bcdc-22000b9a448b.e.globus.org/ds131.2/Data-Reduction-Repo/raw-data/EXAALT/SDRBENCH-EXAALT-2869440.tar.gz
#tar -xvf SDRBENCH-EXAALT-2869440.tar.gz
#wait
cd /users/mlhickm/Compression_Injection-Power_Experiment/
make
wait

./comp_inj /users/mlhickm/Compression_Injection-Power_Experiment/data/SDRBENCH_Data/2869440/xx.f32 0,0,0,0,280953867 ./config/sz_psnr_60.config $1 $2

#./comp_inj ./data/hacc_data/xx.f32 0,0,0,0,280953867 ./config/sz_psnr_60.config byte_loc 	     flip_loc
#										(0 to compress size) (0 to 7)

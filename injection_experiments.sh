#!/bin/bash


#cd /users/mlhickm/Compression_Injection-Power_Experiment/data
#wget https://97235036-3749-11e7-bcdc-22000b9a448b.e.globus.org/ds131.2/Data-Reduction-Repo/raw-data/EXAALT/SDRBENCH-EXAALT-2869440.tar.gz
#tar -xvf SDRBENCH-EXAALT-2869440.tar.gz
#wait
cd /users/mlhickm/Compression_Injection-Power_Experiment/
make
wait

#./comp_inj data_file                                                                               data_size        config_file           byte_loc flip_loc
./comp_inj /users/mlhickm/Compression_Injection-Power_Experiment/data/SDRBENCH_Data/2869440/xx.f32 0,0,0,0,280953867 ./config/sz_psnr_60.config $1 $2

# Look up Table:
#config_file=''
#case $1 in
#   ABS_1e2)
#       config_file = ./config/sz_psnr_60.config
##    ;;
#   ABS_1e3)
#       config_file = ./config/sz_psnr_60.config
##    ;;
#   PW_1e2)
#       config_file = ./config/sz_psnr_60.config
##    ;;
#   PW_1e3)
#       config_file = ./config/sz_psnr_60.config
##    ;;
#   PSNR_30)
#       config_file = ./config/sz_psnr_30.config
##    ;;
#   PSNR_60)
#       config_file = ./config/sz_psnr_60.config
##    ;;
#  *)
#    echo -n "Unknown config file..."
#    ;;
#esac
#./comp_inj /users/mlhickm/Compression_Injection-Power_Experiment/data/SDRBENCH_Data/2869440/xx.f32 0,0,0,0,280953867 ./config/sz_psnr_60.config $1 $2

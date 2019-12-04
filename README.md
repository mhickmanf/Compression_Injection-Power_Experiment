# Compression_Power_Experiment
Experiments involving using RAPL to get power consumption profiles on SZ and ZFP.

comp_inj_abs_pbs.sh
    One of Dakota's scripts I will use as a basis to make my own pbs script later.

comp_inj_runner.py
    One of Dakota's scripts I will use as a basis to may my own runner script later.

comp_profile.c
    Script to compress then decompress a file using SZ. The main file in these experiments.

comp_profile
    generated via: gcc comp_profile.c -o comp_profile
    This is done in the Makefile.
    Called via: /comp_inj data/SZ_test_data/testfloat_8_8_128.dat 0,0,0,0,8192 config/sz.config	

config/
    Folder containing the different configuration files, to make it easier to use with comp_profile.c

data/
    Folder containing the *small* datasets that we can use with SZ in our experiments.

injection_experiments.sh
    Bash script that will run a certain fault injection experiment based on inputs.
    This file is called by rapl_plot/4-Subprocess.py so that the rapl power data is recorded for the experiment specified here.
    
    $ bash injection_experiments.sh [experiment_type] [byte_location] [flip_location]
    Where experiment_type can be: IDLE, ABS_1e2, ABS_1e3, PW_1e2, PW_1e3, PSNR_30, PSNR_60
    
    Example calls:
        $ bash injection_experiments.sh IDLE
        $ bash injection_experiments.sh ABS_1e2 77 3

Makefile
    The makefile to create comp_profile.

rapl_plot/
    Folder containing files to gather power/energy consumption over time.

SZ_Files/
    Placeholder to hold files from the SZ github page:
        git clone https://github.com/disheng222/SZ.git

test_results/
    Folder containing results of experiments.

V1_comp_profile.c
    Original file to compress then decompress a file using SZ.
    **NO LONGER BEING USED**
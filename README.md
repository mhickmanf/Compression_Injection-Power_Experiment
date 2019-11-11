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
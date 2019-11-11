#!/bin/bash

cd
cd Compression_Power_Experiment
make

./comp_inj data/SZ_test_data/testfloat_8_8_128.dat 0,0,0,0,8192 config/sz.config	
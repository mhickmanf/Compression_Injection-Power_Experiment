#!/bin/bash


module load python/3.4
python3 comp_inj_runner.py ./data/sz_test_data/testfloat_8_8_128.dat 0,0,0,0,8192 ./config/sz_abs_1e7.config sz_abs_1e7_results.csv abs

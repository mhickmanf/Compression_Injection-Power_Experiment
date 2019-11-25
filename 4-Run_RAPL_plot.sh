#!/bin/bash

# Get its PID so we know how to kill this process
#PID=$!

cd /users/mlhickm/Compression_Power_Experiment/rapl_plot
make

# for loop of numbers
# experiment_id

python3 4-Subprocess.py 1-results.PACKAGE_ENERGY_PACKAGE0 1-results.PACKAGE_ENERGY_PACKAGE1 1-results.DRAM_ENERGY_PACKAGE0 1-results.DRAM_ENERGY_PACKAGE1 1-results.PP0_ENERGY_PACKAGE0 1-results.PP0_ENERGY_PACKAGE1 1-results.UNCORE_PACKAGE0 1-results.UNCORE_PACKAGE1

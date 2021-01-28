#!/bin/bash

# Get its PID so we know how to kill this process
#PID=$!

cd /users/mlhickm/Compression_Injection-Power_Experiment/rapl_plot
make

# for loop of numbers
# experiment_id
yes | rm -r power_results
mkdir power_results

# Compression information
#['ABS_1e2','ABS_1e3','PW_1e2','PW_1e3','PSNR_30','PSNR_60']
#exp='IDLE'
#exp='ABS_1e2'
#exp='ABS_1e3'
#exp='PW_1e2'
#exp='PW_1e3'
#exp='PSNR_30'
exp='PSNR_60'

counter=1
while [ $counter -le 5 ]
do
	echo $counter
	python3 4-Subprocess.py power_results/$counter-results.PACKAGE_ENERGY_PACKAGE0 power_results/$counter-results.PACKAGE_ENERGY_PACKAGE1 power_results/1-results.DRAM_ENERGY_PACKAGE0 power_results/1-results.DRAM_ENERGY_PACKAGE1 power_results/1-results.PP0_ENERGY_PACKAGE0 power_results/1-results.PP0_ENERGY_PACKAGE1 power_results/1-results.UNCORE_PACKAGE0 power_results/1-results.UNCORE_PACKAGE1 $exp
	# We only update variable name for package info bc that is all we are evaulating at the moment
	((counter++))
	killall comp_inj
done
#killall comp_inj
echo All done



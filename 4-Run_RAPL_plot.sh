#!/bin/bash

# Get its PID so we know how to kill this process
#PID=$!

cd /users/mlhickm/Compression_Injection-Power_Experiment/rapl_plot
make

# for loop of numbers
# experiment_id
mkdir exaalt_results

counter=1
while [ $counter -le 5 ]
do
	echo $counter
	((counter++))
	python3 4-Subprocess.py exaalt_results/$counter-results.PACKAGE_ENERGY_PACKAGE0 exaalt_results/$counter-results.PACKAGE_ENERGY_PACKAGE1 exaalt_results/1-results.DRAM_ENERGY_PACKAGE0 exaalt_results/1-results.DRAM_ENERGY_PACKAGE1 exaalt_results/1-results.PP0_ENERGY_PACKAGE0 exaalt_results/1-results.PP0_ENERGY_PACKAGE1 exaalt_results/1-results.UNCORE_PACKAGE0 exaalt_results/1-results.UNCORE_PACKAGE1
	# We only update variable name for package info bc that is all we are evaulating at the moment
done
echo All done


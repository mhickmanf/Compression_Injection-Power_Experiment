<h1>Test Results</h1>

Directory of completed test results stored for future use.

plot.py
    This script will take in input files as arguements, then calculate the average power for each (which are saved to an output file), followed by the average of all of the averages. If specified, it will make a plot for each input file for power consumption over time.

    % python3 plot.py "test/results.PACKAGE_ENERGY_PACKAGE0","test/results.PACKAGE_ENERGY_PACKAGE1" yes output_file.csv
	% python3 plot.py "test/results.PACKAGE_ENERGY_PACKAGE0" no output_file.csv


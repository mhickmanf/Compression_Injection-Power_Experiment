# Example Call
	# python3 plot.py "test/results.PACKAGE_ENERGY_PACKAGE0","test/results.PACKAGE_ENERGY_PACKAGE1" yes output_file.csv
	# python3 plot.py "test/results.PACKAGE_ENERGY_PACKAGE0" no output_file.csv
import numpy as np
import matplotlib.pyplot as plt

import pandas as pd 
import seaborn as sns

import sys

def str2bool(v):
	if isinstance(v, bool):
		return v
	if v.lower() in ('yes', 'true', 't', 'y', '1'):
		print("We will create a plot.")
		return True
	elif v.lower() in ('no', 'false', 'f', 'n', '0'):
		print("We will not create a plot.")
		return False
	else:
		print("Boolean value expected.")
		exit(0)

def main():

	plot = str2bool(sys.argv[2]) # True or False should we make a plot
	
	input_files = sys.argv[1].split(',')
	output_file = open(sys.argv[3], 'a')

	for input_file in input_files:
		print ("Input File: ", input_file)
		data = pd.read_csv(input_file, header=None, delim_whitespace=True) 
		data = data.iloc[:, 0:2]
		
		# Average power for this run
		average_power = data[1].mean()
		#print("Average power = ",average_power,"\n")

		# Save this to a file
		output_file.write("%s\n" % average_power)

		if plot == True:
			max_power = data[1].max()
			#print("Max power =", max_power)

			x = data.iloc[:,0]
			y = data.iloc[:,1]
			#colors = (0,0,0)
			#area = np.pi*3

			sns.set_style("darkgrid")
			plt.plot(x,y)
			plt.ylim(0, max_power + 5)
			#plt.show()
			plt.savefig(input_file+".png")

	output_file.close()

	# Calculate average of averages (total average of experiment)
	data2 = pd.read_csv(sys.argv[3], header=None) 
	average_average_power = data2[0].mean()
	print("Average power of experiments = ",average_average_power,"\n")
if __name__ == '__main__':
	main()

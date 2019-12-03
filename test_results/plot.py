# Example Call
	# python3 plot.py [plot] [EXTENSION - find file ending in] [FOLDER - to search in]
	# python3 plot.py yes results.PACKAGE_ENERGY_PACKAGE1 EXPERIMENT_1/HACC/control_hacc/ABS_1e2/exaalt_results
	# python3 plot.py no output_file.csv IDLE/exaalt_results results.PACKAGE_ENERGY_PACKAGE1

# Creates file "power_output.csv" that contains average power of each trial

import numpy as np
import matplotlib.pyplot as plt

import pandas as pd 
import seaborn as sns
import os
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

	plot = str2bool(sys.argv[1]) # True or False should we make a plot
	#EXTENSION = "results.PACKAGE_ENERGY_PACKAGE1"
	EXTENSION = sys.argv[2]
	#FOLDER = "IDLE/exaalt_results"
	#FOLDER = "EXPERIMENT_1/HACC/control_hacc/ABS_1e2/exaalt_results"
	FOLDER = sys.argv[3]

	output_filename = "power_output.csv"
	# Delete file if it exists, to not apend to old data
	try:
		os.remove(output_filename)
	except:
		pass

	try:
		output_file = open(output_filename,'a')
	except:
		print("File cannot be opened!", output_filename)
		exit(0)

	for input_file in os.listdir(FOLDER):
		if input_file.endswith(EXTENSION):
			input_file = os.path.join(FOLDER, input_file)
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
	data2 = pd.read_csv("power_output.csv", header=None) 
	#average_average_power = data2[0].mean()
	#standard_deviation_power = data[0].std()

	# SAVE TO A FILE?????

	print("/----- *** Results *** -----/")
	print(FOLDER, EXTENSION)
	print("Average power of experiments = ",data2[0].mean())
	print("Maximum power of experiments = ",data2[0].max())
	print("Minimum power of experiments = ",data2[0].min())
	print("Standard Deviation           = ",data2[0].std())
	print("/--- *** End Results *** ---/\n")
if __name__ == '__main__':
	main()


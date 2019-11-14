import numpy as np
import matplotlib.pyplot as plt

import pandas as pd 
import seaborn as sns

def main():

	plot = True # Make True if you want to make a plot
	
	data = pd.read_csv("test/results.PACKAGE_ENERGY_PACKAGE0", header=None, delim_whitespace=True) 
	data = data.iloc[:, 0:2]
	#print(data.head())
	
	average_power = data[1].mean()
	print("Average power = ",average_power)

	# We will want to save this to a file later


	if plot == True:
		max_power = data[1].max()
		#print("Max power =", max_power)

		x = data.iloc[:,0]
		y = data.iloc[:,1]
		colors = (0,0,0)
		#area = np.pi*3

		sns.set_style("darkgrid")
		plt.plot(x,y)
		plt.ylim(0, max_power + 5)
		plt.show()


if __name__ == '__main__':
	main()

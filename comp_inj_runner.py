import concurrent.futures
import subprocess
import re
from datetime import datetime
import time
import math
import os
import sys

# Calls C Program, checks to see if it finished near instantly.
# If not then wait the timeout period and check every .1 seconds.
def popen_timeout(command, timeout):
	p = subprocess.Popen(command, stdout=subprocess.PIPE)
	time.sleep(.03)
	if p.poll() is not None:
		return p.communicate(), p.pid
	else:
		for t in range(timeout*10):
			time.sleep(.1)
			if p.poll() is not None:
				return  p.communicate(), p.pid
		p.kill()
		return ["Timeout"], p.pid


# Call C Program and write results to the output file.
def experiment(chunk_id, data_path, data_dimension_input, compression_size, config, start, end, temp_file_prefix):

	# Get output information to save results
	output_file = "results/{}_chunk_{}_results.csv".format(temp_file_prefix, chunk_id)
	output = open(output_file, "w")
	output.write("DataSize, CompressedSize, ErrorInfo, ByteLocation, FlipLocation, DecompressionTime, Incorrect, MaxDifference, MaxDeviation, RMSE, PSNR, Status, Traceback\n")

	print("Running Trials. . .\n")
	print("Hitting {} to {}".format(start, end))
	for byte in range(start, end+1):
		for bit in range(8):
			# Call c program
			proc_result, child_process_id = popen_timeout(['./comp_inj', data_path, data_dimension_input, config, str(byte), str(bit)], 5)
			response = str(proc_result[0])
		
			if "Timeout" not in response:
				# Parse response	
				error_info = re.findall(r"(?<=Error-Bounding Information: ).+?(?=\\n)", response)[0]
				byte_location = re.findall(r"(?<=Byte Location: ).+?(?=\\n)", response)[0]
				flip_location = re.findall(r"(?<=Flip Location: ).+?(?=\\n)", response)[0]
				data_size = re.findall(r"(?<=Original Data Size in Bytes: ).+?(?=\\n)", response)[0]	
				compressed_size = re.findall(r"(?<=Compressed Size in Bytes: ).+?(?=\\n)", response)[0]
				try:
					time_taken = re.findall(r"(?<=Time to Decompress: ).+?(?=\\n)", response)[0]
					num_incorrect = re.findall(r"(?<=Number of Incorrect: ).+?(?=\\n)", response)[0]
					abs_diff = re.findall(r"(?<=Maximum Absolute Difference: ).+?(?=\\n)", response)[0]
					abs_dev = re.findall(r"(?<=Maximum Absolute Deviation: ).+?(?=\\n)", response)[0]
					rmse = re.findall(r"(?<=Root Mean Squared Error: ).+?(?=\\n)", response)[0]
					psnr = re.findall(r"(?<=PSNR: ).+?(?=\\n)", response)[0]
					status = "Completed"	
					traceback = "NA"
				except:
					core_file = "core.{}".format(child_process_id)
					if os.path.exists(core_file):	
						time_taken = "-1"
						num_incorrect = "-1"
						abs_diff = "-1"
						abs_dev = "-1"
						rmse = "-1"
						psnr = "-1"
						status = "CoreDump"
						traceback = "NA"
						os.system("rm {}".format(core_file))	
					elif "Wrong version" in response:
						time_taken = "-1"
						num_incorrect = "-1"
						abs_diff = "-1"
						abs_dev = "-1"
						rmse = "-1"
						psnr = "-1"
						status = "VersionError"
						traceback = "NA"
					elif "Sig 11" in response:
						time_taken = "-1"	
						num_incorrect = "-1"
						abs_diff = "-1"
						abs_dev = "-1"
						rmse = "-1"
						psnr = "-1"
						status = "SegFault"
						try:
							traceback = re.findall(r"(?<=Receiving Sig 11: ).+?(?=\\n)", response)[0]
							traceback = re.findall(r"\/+.*?\)+", traceback)
							traceback = ' <- '.join(traceback)	
						except:
							traceback = "Unknown"
					elif "stepLength" in response:
						time_taken = "-1"	
						num_incorrect = "-1"
						abs_diff = "-1"
						abs_dev = "-1"
						rmse = "-1"
						psnr = "-1"
						status = "StepLengthError"
						traceback = "NA"
					else:
						print("Unkown Output\n")
						print(child_process_id)
						print(response)
						time_taken = "-1"
						num_incorrect = "-1"
						abs_diff = "-1"
						abs_dev = "-1"
						rmse = "-1"
						psnr = "-1"
						status = "Unknown"
						traceback = "NA"
			else:
				error_info = "NA"
				byte_location = str(byte)
				flip_location = str(bit)
				data_size = "NA"
				compressed_size = "NA"
				time_taken = "-1"
				num_incorrect = "-1"
				abs_diff = "-1"
				abs_dev = "-1"
				rmse = "-1"
				psnr = "-1"
				status = "Timeout"
				traceback = "NA"
	
			# Print out to file
			output_row = "{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}\n".format(data_size, compressed_size, error_info, byte_location, flip_location, time_taken, num_incorrect, abs_diff, abs_dev, rmse, psnr, status, traceback)
			output.write(output_row)
			output.flush()
		
	#Close output file
	output.close()


def main():
	if len(sys.argv) != 4:
		print("Incorrect Number of Arguements. . .")
		exit(-1)

	# Get input needed to run the experiment	
	data_path = sys.argv[1].strip()
	data_dimension_input = sys.argv[2].strip()
	config = sys.argv[3].strip()
	output_file_name = sys.argv[4].strip()
	temp_file_prefix = sys.argv[5].strip()

	print("Starting Experiment:")
	startTime = datetime.now()

	# Determine Compressed Size
	proc = subprocess.Popen(['./comp_inj', data_path, data_dimension_input, config], stdout=subprocess.PIPE)
	response = str(proc.communicate()[0])
	compression_size = int(re.findall(r"(?<=Compressed Size in Bytes: ).+?(?=\\n)", response)[0])
	print("Compressed Size of Data in Bytes: {}".format(compression_size))

	

if __name__ == '__main__':
	main()

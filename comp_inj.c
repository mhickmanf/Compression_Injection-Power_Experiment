#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include <execinfo.h>
#include "sz.h"
#include "rw.h"


//GLOBAL VARIABLES

// Initial Data Pointer
float *DATA;
// Decompressed Data Pointer
float *RET_DATA;

/*
 * Function: sigHandler
 * -------------------------------------------------------------------------------
 * Handles segmantation faults due to decompression errors.
 *
 * sig: the signal value of the error.
 * -------------------------------------------------------------------------------
 */
void sigHandler(int sig) {
	printf("Receiving Sig %d: ", sig);
	void *buffer[512];
	char **strings;
	int j, nptrs;

	nptrs = backtrace(buffer, 512);

	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL){
		printf("Error with backtrace_symbols\n");
		exit(-1);
	}

	for (j = 0; j < nptrs; j++){
		printf("%s <- ", strings[j]);
	}
	printf("\n");
	free(strings);

	if (DATA){
		free(DATA);
	}
	if (RET_DATA){
		free(RET_DATA);
	}
	exit(sig);
}

/*
 * Function: startsWith
 * -------------------------------------------------------------------------------
 * Checks if one string starts with a substring
 *
 * returns: True or False
 * -------------------------------------------------------------------------------
 */
int startsWith(const char *a, const char *b) {
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}

/*
 * Function: printBits
 * -------------------------------------------------------------------------------
 * Prints the bits of a given variable.
 *
 * size: the sizeof the variable
 * ptr: the pointer to the variable
 * -------------------------------------------------------------------------------
 */
void printBits(size_t const size, void const * const ptr){
	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;
	int i, j;

	for (i=size-1;i>=0;i--){
		for (j=7;j>=0;j--){
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
}


/*
 * Function: szCompression
 * -------------------------------------------------------------------------------
 * Compresses the given data and returns the new decompressed data.
 *
 * config: Config file to use for compression and decompression.
 * data_dimensions: Array of the 5 dimensions of the data.
 * -------------------------------------------------------------------------------
 */
void szCompression(char *config, int *data_dimensions){	
	// Get Data Dimensions
	int r5 = data_dimensions[0];
	int r4 = data_dimensions[1];
	int r3 = data_dimensions[2];
	int r2 = data_dimensions[3];
	int r1 = data_dimensions[4];

	// Initialize SZ
	int status = SZ_Init(config);

	if (status != SZ_SCES){
		printf("Error, SZ not initialized. . .\n");
		exit(-1);
	}else {
		if (DEBUG){
			printf("SZ Successfully Initialized!\n");
		}

		// Compress the data here
		size_t outsize;
		gettimeofday(&start, NULL);
		unsigned char *bytes = SZ_compress(SZ_FLOAT, DATA, &outsize, r5, r4, r3, r2, r1);
		gettimeofday(&stop, NULL);
		double time_taken = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
		printf("Time to Compress: %lf\n", time_taken);

		printf("Compressed Size in Bytes: %zu\n", outsize);

		if (DEBUG){
			printf("Successfully compressed data\n");

			// Print out correct compressed value of that spot
			printf("Previous Value: ");
			printBits(sizeof(bytes[char_loc]), &bytes[char_loc]);
			printf("\n");
		}

		// Decompress the data here
		struct timeval start, stop;
		gettimeofday(&start, NULL);
		RET_DATA = SZ_decompress(SZ_FLOAT, bytes, outsize, r5, r4, r3, r2, r1);
		gettimeofday(&stop, NULL);
		double time_taken = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
		printf("Time to Decompress: %lf\n", time_taken);
		if (DEBUG) {
			printf("Successfully decompressed data\n");
		}
	}
}

/* 
 * Function: main
 * -------------------------------------------------------------------------------
 * Takes user input and will compress data, then decompress it. 
 *
 * Example Call: 
 * ./comp_inj	data_path				 data_dimensions	config
 * ./comp_inj	./xx.dat2 				 0,0,0,0,2869440	sz_abs.config
 * ./comp_inj	./testfloat_8_8_128.dat  0,0,0,0,8192		sz.config
 * 
 * NOTE: This code only works with 1d, so we will read in the multidimensional
 * array as a 1d by specifiying so in the data_dimensions variable.
 * 
 * data_path: Full path to the data file (/Compression_Power_Experiment/data/XX)
 * data_size: Dimensions of your data (r5,r4,r3,r2,r1)
 * config: SZ config file (/Compression_Power_Experiment/config/XX.config)
 * -------------------------------------------------------------------------------
 */
int main(int argc, char *argv[]){
	int i;
	//Catches segmentation faults and other signals
	if (signal (SIGSEGV, sigHandler) == SIG_ERR){
        	printf("Error setting segfault handler...\n");
	}

	printf("Starting Experiment\n");

	// PARSE USER INPUT
	// *******************
	char *data_path; 
	char *data_dimensions_string;
	int data_dimensions[5];
	int data_size = 1;
	char *config;

	if (argc == 4){
		//Get Binary Data File Path
		data_path = argv[1];
		//Parse data dimensions from string
		data_dimensions_string = argv[2];
		char *pt;
		pt = strtok(data_dimensions_string, ",");
		int count = 0;
		while (pt != NULL) {
			data_dimensions[count] = atoi(pt);
			count++;
			pt = strtok (NULL, ",");
		}
		//Determine Data Size
		for (i = 0; i < 5; i++){
			if (data_dimensions[i] != 0){
				data_size = data_size * data_dimensions[i];
			}	
		}
		//Get config file path
		config = argv[3];
	} else {
		printf("Incorrect Number of Command Line Arguements Passed. . .\n");
		exit(-1);
	}

	// Attempt to open sz config file
	FILE *config_file;
	if ((config_file = fopen(config,"r")) == NULL){
		printf("Cannot open sz config file. . .");
		exit(-1);
	}else {
		if (DEBUG) {
			printf("sz config file opened successfully!")
		}
	}
	

	// Pull algorithm and error information from config file
	char compression_algorithm[16] = "NULL";
	float error_information;
	char *line;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, config_file)) != -1) {
		if(startsWith(line, "errorBoundMode")){
			sscanf(line, "errorBoundMode = %s ", compression_algorithm);
		}else if(startsWith(compression_algorithm, "ABS")){
			if(startsWith(line, "absErrBound")){
				sscanf(line, "absErrBound = %f ", &error_information);
			}
		}else if(startsWith(compression_algorithm, "PSNR")){
			if(startsWith(line, "psnr")){
				sscanf(line, "psnr = %f ", &error_information);
			} 
		}else if(startsWith(compression_algorithm, "PW_REL")){
			if(startsWith(line, "pw_relBoundRatio")){
				sscanf(line, "pw_relBoundRatio = %f ", &error_information);
			}
		}
	}

	// COMPRESS & DECOMPRESS
	// *******************
	// Read data from binary file
	FILE *fp;
	DATA= malloc(sizeof(float) * data_size);
	fp = fopen(data_path,"rb");
	if (fp == NULL){
		perror("ERROR: ");
		exit(-1);
	} else {
		fread(DATA, 4,  data_size, fp);
		fclose(fp);
	}

	// Print out all parameters
	printf("Data File: %s\n", data_path);
	printf("Data Dimensions: %d x %d x %d x %d x %d\n", data_dimensions[0], data_dimensions[1], data_dimensions[2], data_dimensions[3], data_dimensions[4]);
	printf("Config File: %s\n", config);
	printf("Compression Algorithm: %s\n", compression_algorithm);
	printf("Error-Bounding Information: %f\n", error_information);
	printf("Original Data Size in Bytes: %ld\n", sizeof(float)*data_size);

	// Call compression injection function
	szCompression(config, data_dimensions, char_loc, flip_loc);

	// Print small before and after if debugging is turned on
	if (DEBUG){	
		printf("Original Data:\n");
		for (i = 0; i < 10; i++){
			printf("%f\n", DATA[i]);
		}
		printf("New data:\n");
		for (i = 0; i < 10; i++){
			printf("%f\n", RET_DATA[i]);
		}
	}

	// CALCULATE METRICS
	/* *******************
	int number_of_incorrect = 0;
	float max_diff = 0;
	float max_deviation = 0;
	float rmse_sum = 0;
	float max_val = -1;
	float min_val = -1;
	*/


	if(DATA){
		free(DATA);
	}
	if (RET_DATA){
		free(RET_DATA);
	}
	printf("End of Experiment\n");
	return 0;
}

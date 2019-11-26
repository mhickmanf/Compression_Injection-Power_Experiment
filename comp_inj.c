#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <float.h>
#include <sys/time.h>
#include <execinfo.h>
#include "sz.h"
#include "rw.h"

/*
 * GLOBAL VARIABLES
 */
// Print out extra options (1 for True, 0 for False)
int DEBUG = 0;
// Inject into the compression (1 for True, 0 for False)
int INJECT = 1;

// Initial Data Pointer
float *DATA;
// Faulted Decompressed Data Pointer
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
 * Function: szCompressionInjection
 * -------------------------------------------------------------------------------
 * Compresses the given data, injects a fault based on given
 * parameters, and returns the new decompressed data.
 *
 * config: Config file to use for compression and decompression.
 * data_dimensions: Array of the 5 dimensions of the data.
 * char_loc: The byte position in the commpressed data 
 * to inject into.
 * flip_loc: The bit of the chosen byte to flip.
 * -------------------------------------------------------------------------------
 */
void szCompressionInjection(char *config, int *data_dimensions, int char_loc, int flip_loc){	
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
		unsigned char *bytes = SZ_compress(SZ_FLOAT, DATA, &outsize, r5, r4, r3, r2, r1);
		
		printf("Compressed Size in Bytes: %zu\n", outsize);

		if (DEBUG){
			printf("Successfully compressed data\n");

			// Print out correct compressed value of that spot
			printf("Previous Value: ");
			printBits(sizeof(bytes[char_loc]), &bytes[char_loc]);
			printf("\n");
		}

		// Generate flip mask based on flip_loc
		int mask = 0;
		int one = 1;
		mask = mask | (one << flip_loc);

		if (DEBUG){		
			printf("Mask: ");
			printBits(sizeof(bytes[char_loc]), &mask);
			printf("\n");
		}

		// XOR with area of byte array and save back to the array
		if (INJECT){
			int new_char = (int) bytes[char_loc] ^ mask;
			bytes[char_loc] = (char) new_char;
		}
		
		if (DEBUG){
			// Print out the new value
			printf("New Value: ");
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
 * Takes user input and will compress data, inject a fault into the compressed 
 * data, and attempt to decompress it. Metrics on the outcome of the decompression
 * will be printed out throughout the process.
 *
 * Example Call: 
 * ./comp_inj	data_path	data_dimensions	config			char_loc	bit_loc
 * ./comp_inj	./xx.dat2 	0,0,0,0,2869440	sz_abs.config	101 		7
 *
 * data_path: Full path to the data file
 * data_size: Dimensions of your data (r5,r4,r3,r2,r1)
 * config: SZ config file
 * char_loc: Which byte in the resulting char compression to inject into.
 * NOTE: To determine char_loc range, run ./comp_inj once with char_loc 0 and
 * bit_loc 0 to get 'Compressed Size in Bytes' value.
 * flip_loc: Which bit in the chosen byte to flip.
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
	int char_loc = 0;
	int flip_loc = 0;

	if (argc == 6){
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
		//Get which byte to hit
		char_loc = atoi(argv[4]);
		//Get which bit to flip
		flip_loc = atoi(argv[5]);
		if (flip_loc > 7){
			printf("ERROR: Flip Range Out of Bounds. . . \n");
			exit(-1);
		}
	} else {
		printf("Incorrect Number of Command Line Arguements Passed. . .\n");
		exit(-1);
	}

	// Attempt to open sz config file
	FILE *config_file;
	if ((config_file = fopen(config,"r")) == NULL){
		printf("Cannot open sz config file. . .");
		exit(-1);
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

	// COMPRESS & INJECT
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
	printf("Error-Bounding Information: %0.12f\n", error_information);
	printf("Byte Location: %d\n", char_loc);
	printf("Flip Location: %d\n", flip_loc);
	printf("Original Data Size in Bytes: %ld\n", sizeof(float)*data_size);

	// Call compression injection function
	szCompressionInjection(config, data_dimensions, char_loc, flip_loc);

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
	// *******************
	int number_of_incorrect = 0;
	float max_diff = 0;
	float rmse_sum = 0;
	float max_val = -1;
	float min_val = -1;

	if(startsWith(compression_algorithm, "ABS")){
		for (i = 0; i < data_size; i++){
			float a = DATA[i];
			float b = RET_DATA[i];

			// RMSE Work
			float rmse_diff = a - b;
			rmse_diff = rmse_diff * rmse_diff;
			rmse_sum = rmse_sum + rmse_diff;
			
			//PSNR Work
			if(a > max_val || max_val == -1){
				max_val = a;
			}
			if(a < min_val || min_val == -1){
				min_val = a;
			}

			float diff = fabs(a - b);
			if(diff > error_information){
				if (DEBUG){
					printf("Before: %f\n", a);
					printf("After: %f\n", b);
					printf("Difference: %f\n", diff);
					printf("Err Bound:  %f\n", error_information);
					return 0;
				}
				number_of_incorrect++;
			}
			if(diff > max_diff){
				max_diff = diff;
			}
		}
	}else if(startsWith(compression_algorithm, "PSNR")){
		number_of_incorrect = -1;
		for (i = 0; i < data_size; i++){
			float a = DATA[i];
			float b = RET_DATA[i];

			// RMSE Work
			float rmse_diff = a - b;
			rmse_diff = rmse_diff * rmse_diff;
			rmse_sum = rmse_sum + rmse_diff;
			
			//PSNR Work
			if(a > max_val || max_val == -1){
				max_val = a;
			}
			if(a < min_val || min_val == -1){
				min_val = a;
			}

			float diff = fabs(a - b);
			if(diff > max_diff){
				max_diff = diff;
			}	
		}
	}else if(startsWith(compression_algorithm, "PW_REL")){
		for(i = 0; i < data_size; i++){
			float a = DATA[i];
			float b = RET_DATA[i];

			// RMSE Work
			float rmse_diff = a - b;
			rmse_diff = rmse_diff * rmse_diff;
			rmse_sum = rmse_sum + rmse_diff;
			
			//PSNR Work
			if(a > max_val || max_val == -1){
				max_val = a;
			}
			if(a < min_val || min_val == -1){
				min_val = a;
			}
		
			float diff = fabs(a - b);
			float rel_bound = fabs(error_information * a);
			if(diff > rel_bound){
				if (DEBUG){
					printf("Before: %f\n", a);
					printf("After: %f\n", b);
					printf("Difference: %f\n", diff);
					printf("Rel Bound:  %f\n", rel_bound);
					return 0;
				}
				number_of_incorrect++;
			}
			if (diff > max_diff){
				max_diff = diff;
			}				
		}
	}

	//Calculate Root Mean Square Error 
	float rmse = rmse_sum / (data_size - 1);
	rmse = sqrt(rmse);

	//Check RMSE for bad values
	if (fpclassify(rmse) == FP_INFINITE) {
		rmse = FLT_MAX;
	} else if (fpclassify(rmse) == FP_NAN) {
		rmse = FLT_MAX;
	}

	//Calculate PSNR
	float psnr_control_value = 10000;
	float psnr = 0;
	if (rmse == 0){
		psnr = psnr_control_value;
	} else {
		psnr = 20 * log10((max_val - min_val) / rmse);	
	}	

	//Check PSNR for bad values
	if (fpclassify(psnr) == FP_INFINITE){
		psnr = psnr_control_value * -1;
	} else if (fpclassify(psnr) == FP_NAN) {
		psnr = psnr_control_value * -1;
	}
	
	//Check Maximum Difference for bad values
	if (fpclassify(max_diff) == FP_INFINITE){
		max_diff = FLT_MAX;
	} else if (fpclassify(max_diff) == FP_NAN) {
		max_diff = FLT_MAX;
	}

	//Print Metrics
	printf("Number of Incorrect: %d\n", number_of_incorrect);
	printf("Maximum Absolute Difference: %f\n", max_diff);
	printf("Root Mean Squared Error: %f\n", rmse);
	printf("PSNR: %f\n", psnr);

	if(DATA){
		free(DATA);
	}
	if (RET_DATA){
		free(RET_DATA);
	}
	printf("End of Experiment\n");
	return 0;
}

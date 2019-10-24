#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "sz.h"
#include "rw.h"

/* This file will read in a binary.dat file, print the input in human-readable format, compress data, decompress data, and print results
 *
 */


const char *data_location = "testfloat_8_8_128.dat";

void sigHandler(int sig) {
    printf("Receiving Sig %d\n", sig);
    exit(sig);
}

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



int main(){
	if (signal (SIGSEGV, sigHandler) == SIG_ERR){
        	printf("Error setting segfault handler...\n");
	}

	// Read in ya data
	char data_path[512] = "/users/mlhickm/rapl_power_monitoring/compression_profiles_experiment/SZ_test_data/";
	//float data[128];
	int data_size = 8192;
	//float data[2000000];
	
	float *data = malloc(sizeof(float) * data_size);
	// Read in binary data from the different files;
	FILE *fp;

	//char *data_file = strcat(data_path, "testfloat_8_8_128.dat");
	char *data_file = strcat(data_path, data_location);
	printf("Data File Located at: %s\n", data_file);

	fp = fopen(data_file,"rb");
	// Store data for later comparison
	fread(data,sizeof(float), data_size,fp);
	printf("data[0]: %f\n", data[0]);
	printf("data[5]: %f\n", data[5]);
	
	// Print out the top few rows of the binary data
	int i;
	printf("Original Data:\n");
	for (i = 0; i < 32; i++){
		printf("%f\n", data[i]);
	}

	// Use SZ to compress the data using the C api
	// Initialize SZ
	int status = SZ_Init("sz_abs_1e7.config");



	
	
	if (status != SZ_SCES){
		printf("Error, SZ not initialized. . .\n");
	}else {
		printf("SZ Successfully Initialized!\n");

		// Compress the data here
		size_t outsize;
		unsigned char *bytes = SZ_compress(SZ_FLOAT, data, &outsize, 0, 0, 8, 8, 128);
		printf("Successfully compressed data\n");

		// Decompress the data here
		float *data2 = SZ_decompress(SZ_FLOAT, bytes, outsize, 0, 0, 8, 8, 128);
		printf("Successfully decompressed data\n");

		printf("New data:\n");
		for (i = 0; i < 32; i++){
			printf("%f\n", data2[i]);
		} 
	}		
	
	return 0;
}

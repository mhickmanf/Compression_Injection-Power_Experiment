/** 
 * @author  Vince Weaver
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "papi.h"

#define NUM_EVENTS 6

char events[NUM_EVENTS][BUFSIZ]={
  "PACKAGE_ENERGY:PACKAGE0",
  "PACKAGE_ENERGY:PACKAGE1",
  "DRAM_ENERGY:PACKAGE0",
  "DRAM_ENERGY:PACKAGE1",
  "PP0_ENERGY:PACKAGE0",
  "PP0_ENERGY:PACKAGE1",
};
FILE *fff[NUM_EVENTS+2];


int main (int argc, char **argv)
{

    if (argc < NUM_EVENTS+1) // expecting NUM_EVENTS file names
            return -1;

    int retval,cid,rapl_cid=-1,numcmp;
    int EventSet = PAPI_NULL;
    long long values[NUM_EVENTS];
    int i;
    const PAPI_component_info_t *cmpinfo = NULL;
    long long start_time,before_time,after_time;
    double elapsed_time,total_time;

    /*
    char user_file_name[80];
    strcpy(user_file_name,  argv[1]);
    strcat(user_file_name, "_results.PACKAGE_ENERGY_PACKAGE0");
    printf("Command line arges %s\n", user_file_name);
    */
   /*
   FILE *inputFile = fopen(user_file_name, "w");
   if (inputFile==NULL) {
	   fprintf(stderr,"Could not open: %s ||\n", user_file_name);
           exit(1);
   }else{
	   printf("SUCES open userfilename\n\n\n\n\n");
   }
   */
   //strncpy(filenames[0][0], user_input_file, 18);

   // results.PACKAGE_ENERGY_PACKAGE0
   /*
   char filenames[NUM_EVENTS+2][BUFSIZ]={
     "results.PACKAGE_ENERGY_PACKAGE0",
     "results.PACKAGE_ENERGY_PACKAGE1",
     "results.DRAM_ENERGY_PACKAGE0",
     "results.DRAM_ENERGY_PACKAGE1",
     "results.PP0_ENERGY_PACKAGE0",
     "results.PP0_ENERGY_PACKAGE1",
     "results.UNCORE_PACKAGE0",
     "results.UNCORE_PACKAGE1",
   };
   */
   /*
   printf("name of file first: %s||\n\n", filenames[2]);
   FILE *inputFile2 = fopen(filenames[2], "w");
   if (inputFile2==NULL) {
           fprintf(stderr,"Could not open: filenames[2]:  %s ||\n", filenames[1]);
           exit(1);
   }else{
           printf("SUCES open filenames[1]\n\n\n\n\n");
   }
   */

	/* PAPI Initialization */
     retval = PAPI_library_init( PAPI_VER_CURRENT );
     if ( retval != PAPI_VER_CURRENT ) {
        fprintf(stderr,"PAPI_library_init failed\n");
	exit(1);
     }

     numcmp = PAPI_num_components();
     printf("numcmp = %d\n", numcmp);
     for(cid=0; cid<numcmp; cid++) {
	printf("for loop %d\n", cid);
	if ( (cmpinfo = PAPI_get_component_info(cid)) == NULL) {
	   fprintf(stderr,"PAPI_get_component_info failed\n");
	   exit(1);
	}
	printf("cmpinfo name: %s\n", cmpinfo->name);

	if (strstr(cmpinfo->name,"rapl")) {
	   rapl_cid=cid;
	   printf("Found rapl component at cid %d\n", rapl_cid);

           if (cmpinfo->num_native_events==0) {
	     fprintf(stderr,"No rapl events found\n");
	     exit(1);
           }
	   break;
	}
     }
     printf("cid is now = %d\n", cid);

     /* Component not found */
     if (cid==numcmp) {
        fprintf(stderr,"No rapl component found\n");
        exit(1);
     }

     printf("Trying to open output files\n");
     /* Open output files */
     for(i=0;i<NUM_EVENTS+2;i++) {
        fff[i]=fopen(argv[i+1],"w");
	if (fff[i]==NULL) {
	   fprintf(stderr,"Could not open: file %d,  %s ||\n",i,argv[i+1]);
	   exit(1);
	}else{
		printf("not a problem");
	}
     }

     printf("Trying to create eventset");
				   

     /* Create EventSet */
     retval = PAPI_create_eventset( &EventSet );
     if (retval != PAPI_OK) {
        fprintf(stderr,"Error creating eventset!\n");
     }
     
     printf("out of if ");

     for(i=0;i<NUM_EVENTS;i++) {
	
        retval = PAPI_add_named_event( EventSet, events[i] );
        if (retval != PAPI_OK) {
	   fprintf(stderr,"Error adding event %s\n",events[i]);
	}
     }

  

     start_time=PAPI_get_real_nsec();
     
     printf("Start time %lld\n",start_time);


     int control;
     control = 0;
     while(1) {
	if(control == 300){ // stop after 30 seconds
		printf("YOU ARE TAKING TOO LONG ... QUIT");
		exit(1);
	}
	control++;



        /* Start Counting */
	printf("Start Counting\n");
        before_time=PAPI_get_real_nsec();
        retval = PAPI_start( EventSet);
        if (retval != PAPI_OK) {
           fprintf(stderr,"PAPI_start() failed\n");
	   exit(1);
        }


        usleep(100000);

	printf("Stop Counting\n");
        /* Stop Counting */
        after_time=PAPI_get_real_nsec();
        retval = PAPI_stop( EventSet, values);
        if (retval != PAPI_OK) {
           fprintf(stderr, "PAPI_start() failed\n");
        }

        total_time=((double)(after_time-start_time))/1.0e9;
        elapsed_time=((double)(after_time-before_time))/1.0e9;

	printf("total time: %f\n", total_time);
	printf("elapsed time: %f\n", elapsed_time);

        for(i=0;i<NUM_EVENTS;i++) {

	   fprintf(fff[i],"%.4f %.1f (* Average Power for %s *)\n",
		   total_time,
		   ((double)values[i]/1.0e9)/elapsed_time,
		   events[i]);
	   fflush(fff[i]);
        }
	fprintf(fff[NUM_EVENTS],"%.4f %.1f (* Average Power for %s *)\n",
		   total_time,
		((double)(values[0]-values[4])/1.0e9)/elapsed_time,
		   "UNCORE_PACKAGE0");
	fprintf(fff[NUM_EVENTS+1],"%.4f %.1f (* Average Power for %s *)\n",
		   total_time,
		((double)(values[1]-values[5])/1.0e9)/elapsed_time,
		   "UNCORE_PACKAGE1");
	   fflush(fff[NUM_EVENTS]);
	   fflush(fff[NUM_EVENTS+1]);
     }
		
     return 0;
}


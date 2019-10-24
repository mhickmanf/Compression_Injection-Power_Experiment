## ************************************************************************
##  Makefile for comp_inj that will be used to test injections into
##  the output of the compressed data of SZ and ZFP.

##  PLEASE UPDATE THESE VARIABLES BEFORE COMPILING
SZPATH 	= /users/mlhickm/rapl_power_monitoring/SZ_Files

##  COMPILER 
CC	= gcc
 
##  FLAGS
SZFLAGS	= -I $(SZPATH)/include/ $(SZPATH)/lib/libSZ.a $(SZPATH)/lib/libzlib.a $(SZPATH)/lib/libzstd.a -lm


## TARGETS
all: comp_profile

comp_profile:	comp_profile.c
	$(CC) -Wall -g -o comp_profile comp_profile.c $(SZFLAGS)

clean:
	rm comp_profile


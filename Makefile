## ************************************************************************
##  Makefile for comp_inj that will be used to test injections into
##  the output of the compressed data of SZ and ZFP.

##  PLEASE UPDATE THESE VARIABLES BEFORE COMPILING
SZPATH 	= /users/mlhickm/Compression_Injection-Power_Experiment/SZ_Files

##  COMPILER 
CC	= gcc
 
##  FLAGS
SZFLAGS	= -I $(SZPATH)/include/ $(SZPATH)/lib/libSZ.a $(SZPATH)/lib/libzlib.a $(SZPATH)/lib/libzstd.a -lm


## TARGETS
all: comp_profile comp_inj

comp_profile:	comp_profile.c
	$(CC) -Wall -g -o comp_profile comp_profile.c $(SZFLAGS)

comp_inj: comp_inj.c
	$(CC) -Wall -g -rdynamic -o comp_inj comp_inj.c $(SZFLAGS)

clean:
	rm comp_profile
	rm comp_inj


#!/bin/bash

cd
sudo apt-get update
sudo apt-get install papi-tools -y

git clone https://bitbucket.org/icl/papi.git
cd papi/src
sudo modprobe msr
./configure --with-components="rapl" &&make

cd components/rapl/tests 
sudo ./rapl_basic
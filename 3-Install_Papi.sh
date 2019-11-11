#!/bin/bash

pwd
sudo apt-get update
yes | sudo apt-get install papi-tools

git clone https://bitbucket.org/icl/papi.git
cd papi/src
pwd
sudo modprobe msr
./configure --with-components="rapl" &&make

cd components/rapl/tests 
pwd
sudo ./rapl_basic

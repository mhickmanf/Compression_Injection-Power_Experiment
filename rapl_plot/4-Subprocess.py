import subprocess
import time
import os
import signal
import sys

def main():
    # Start RAPL
    print("start")
    cmd = ['sudo','./rapl_plot', 
            "1-results.PACKAGE_ENERGY_PACKAGE0",
            "1-results.PACKAGE_ENERGY_PACKAGE1",
            "1-results.DRAM_ENERGY_PACKAGE0",
            "1-results.DRAM_ENERGY_PACKAGE1",
            "1-results.PP0_ENERGY_PACKAGE0",
            "1-results.PP0_ENERGY_PACKAGE1",
            "1-results.UNCORE_PACKAGE0",
            "1-results.UNCORE_PACKAGE1",
    ]
    
    
    
    
    SZcmd = ['bash', '/users/mlhickm/Compression_Power_Experiment/2-Test_SZ_Setup.sh']
    
    rapl = subprocess.Popen(cmd, stdin = subprocess.PIPE, stdout=subprocess.PIPE)
    
    # Call suprocess to run SZ
    print("Starting SZ")
    time.sleep(1)
    p = subprocess.Popen(SZcmd, stdin = subprocess.PIPE, stdout=subprocess.PIPE)
    time.sleep(1)
    #print(p.communicate()) # You can use this to confirm that it worked
    print("Ending SZ")

    #p.kill()
    print("end")


if __name__ == '__main__':
	main()


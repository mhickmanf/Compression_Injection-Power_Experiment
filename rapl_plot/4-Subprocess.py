import subprocess
import time
import os
import signal
import sys


def main():
    # Start RAPL
    print("starting experiment")
    cmd = ['sudo','./rapl_plot', 
            sys.argv[1],
            sys.argv[2],
            sys.argv[3],
            sys.argv[4],
            sys.argv[5],
            sys.argv[6],
            sys.argv[7],
            sys.argv[8],
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
    print("ending experiment")


if __name__ == '__main__':
	main()


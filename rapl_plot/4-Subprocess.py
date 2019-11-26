import subprocess
import time
import os
import signal
import sys


# Calls C Program, checks to see if it finished near instantly.
# If not then wait the timeout period and check every .1 seconds.
def popen_timeout(command, timeout):
    p = subprocess.Popen(command, stdout=subprocess.PIPE)
    time.sleep(.03)
    if p.poll() is not None:
        return p.communicate(), p.pid
    else:
        for t in range(timeout*10):
            time.sleep(.1)
            if p.poll() is not None:
                return  p.communicate(), p.pid
        p.kill()
        return ["Timeout"], p.pid

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

    SZcmd = ['bash', '/users/mlhickm/Compression_Injection-Power_Experiment/injection_experiments.sh']
    timeout_limit = 7 # If taking too long bc injection broke stuff, go ahead and kill experiment

    rapl = subprocess.Popen(cmd, stdin = subprocess.PIPE, stdout=subprocess.PIPE)
    
    # Call suprocess to run SZ
    print("Starting SZ")
    time.sleep(1)
    p = subprocess.Popen(SZcmd, stdin = subprocess.PIPE, stdout=subprocess.PIPE)
    #proc_result, child_process_id = popen_timeout(SZcmd, timeout_limit)    
    time.sleep(1)
    #print(p.communicate()) # You can use this to confirm that it worked
    print("Ending SZ")

    #p.kill()
    print("ending experiment")


if __name__ == '__main__':
	main()


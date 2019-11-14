import subprocess
import time
import os
import signal
import subprocess


def main():
    # Start RAPL
    print("start")
    #subprocess.call("./rapl_plot")
    cmd = ['sudo','./rapl_plot']
    #cmd = ['./rapl_plot']
    #cmd = ['ls']
    
    p = subprocess.Popen(cmd, stdin = subprocess.PIPE, stdout=subprocess.PIPE)
    
    #p = subprocess.Popen(cmd, stdout=subprocess.PIPE)

    # Call suprocess to run SZ
    #print("Starting SZ")
    time.sleep(1)
    #for count in range(50):
    #    print("Ending SZ")

    # Stop RAPL
    #p.kill()
    #print(p.communicate())
    #os.killpg(os.getpgid(p.pid), signal.SIGTERM)  # Send the signal to all the process groups
    #p.terminate()
    #kill -9 (p.pid)
    print("end")


if __name__ == '__main__':
	main()


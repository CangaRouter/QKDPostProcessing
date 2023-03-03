import sys
import os
import subprocess
import re
from server import start_server
from client import start_client
key="Not Found"
if sys.argv[1]=="client":
    process = subprocess.Popen(["../ErrorCorrection/Build/Mycascade", "network",sys.argv[2],"original","client","0.6","localhost","5672","guest","guest"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    exit_code = process.wait()
    lines=stdout.decode().split('\n')
    for line in lines:
        if "Client correct key" in line:
            key=re.search(r'[0-1]*$', line).group()
    start_client(key)


elif sys.argv[1]=="server":
    process = subprocess.Popen(["../ErrorCorrection/Build/Mycascade", "network",sys.argv[2],"original","server","0.6","localhost","5672","guest","guest"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    exit_code = process.wait()
    lines=stdout.decode().split('\n')
    for line in lines:
        if "Server correct key" in line:
            key=re.search(r'[0-1]*$', line).group()
    start_server(key)

else:
    print("Wrong arguments")

import math
import sys
import os
import subprocess
import re
import json
from PrivacyAmplification.server import start_server
from PrivacyAmplification.client import start_client
def calcParameters(compression, keyLenght, blocks):
    M = math.ceil(keyLenght / blocks)
    N = math.ceil(M * compression)
    return M, N

path = "."
host = "localhost"
# host="rabbitmq"
time = []
exchanges = []
efficiency = []
compression = float(sys.argv[8])
keyLenght = int(sys.argv[2])
blocks = int(sys.argv[9])
if sys.argv[8] not in ["0.1", "0.2", "0.5"]:
    print("compression must be: 0.1, 0.2 or 0.5")
    exit(-1)
if sys.argv[1] == "client":
    print("Starting Error Correction")
    process = subprocess.Popen(
        ["./ErrorCorrection/Build/Mycascade", "network", sys.argv[2], sys.argv[3], "client", sys.argv[4], host,
         sys.argv[5], sys.argv[6], sys.argv[7]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #    process = subprocess.Popen(["./Mycascade", "network",sys.argv[2],sys.argv[3],"client",sys.argv[4],"rabbitmq",sys.argv[5],sys.argv[6],sys.argv[7]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    exit_code = process.wait()
    print(stdout.decode())
    print(stderr.decode())
    filename = sys.argv[3] + "_" + sys.argv[2] + "_" + "{:.6f}".format(float(sys.argv[4])) + ".json"
    file = open(path + "/" + filename, "r")
    data = json.load(file)
    print("Elapsed time:", data["elapsed_real_time"])
    print("Number of exchanges: ", data["ask_parity_messages"])
    print("Correction efficiency: ", data["unrealistic_efficiency"])
    os.remove(path+"/"+filename)
    M, N = calcParameters(compression, keyLenght, blocks)
    start_client(data["correctKey"], int(sys.argv[5]), sys.argv[6], sys.argv[7], host, M, N, keyLenght)

elif sys.argv[1] == "server":
    print("Starting Error Correction")
    process = subprocess.Popen(
        ["./ErrorCorrection/Build/Mycascade", "network", sys.argv[2], sys.argv[3], "server", sys.argv[4], host,
         sys.argv[5], sys.argv[6], sys.argv[7]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #    process = subprocess.Popen(["./Mycascade", "network",sys.argv[2],sys.argv[3],"server",sys.argv[4],"rabbitmq",sys.argv[5],sys.argv[6],sys.argv[7]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    exit_code = process.wait()
    print(stdout.decode())
    print(stderr.decode())
    filename = "key.json"
    file = open(path + "/" + filename, "r")
    data = json.load(file)
    start_server(data["correctKey"], int(sys.argv[5]), sys.argv[6], sys.argv[7], host)
    os.remove(filename)
else:
    print("Wrong arguments")

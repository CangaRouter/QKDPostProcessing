import math
import sys
import os
import subprocess
import re
import pika
import progressbar
import time
import json

ER_path = "./ErrorCorrection/Build/Mycascade"
#ER_path = "./error_correction/Mycascade"
PA_path = "./PrivacyAmplification/PrivacyAmplificationCPP/build/PrivacyAmplificationCPP"
#PA_path= "./privacy_amplification/PrivacyAmplificationCPP"
path = "."
connection_attempts = 0
er_Times = []
exchanges = []
efficiencies = []
pa_Times = []
endpoint = sys.argv[1]
keysize = sys.argv[2]
variant = sys.argv[3]
qber = sys.argv[4]
port = sys.argv[5]
user = sys.argv[6]
pw = sys.argv[7]
compression = float(sys.argv[8])
pa_blocks = int(sys.argv[9])
runs = int(sys.argv[10])
seq = sys.argv[11]
host = sys.argv[12]
split_key = sys.argv[13]


def calcParameters(compression, keyLenght, blocks):
    M = math.ceil(keyLenght / blocks)
    N = math.ceil(M * compression)
    return M, N


def extract_data():
    filename = variant + "_" + str(keysize) + "_" + "{:.6f}".format(float(qber)) + ".json"
    file = open(path + "/" + filename, "r")
    data = json.load(file)
    er_Times.append(float(data["elapsed_real_time"]))
    exchanges.append(int(data["ask_parity_messages"]))
    efficiencies.append(float(data["unrealistic_efficiency"]))
    return data, filename


def error_Correction(endpoint):
    if split_key == "False":
        process = subprocess.Popen(
            [ER_path, "network", str(keysize), variant, endpoint, "{:.6f}".format(float(qber)),
             host, port,
             user, pw, seq], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        process.wait()
        if stderr.decode() != "":
            print(stderr.decode())
    else:
        process = subprocess.Popen(
            [ER_path, "many", str(int(keysize) / pa_blocks), variant, endpoint, "{:.6f}".format(float(qber)),
             host, port,
             user, pw, str(pa_blocks), seq], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        process.wait()
        if process.returncode != 0:
            print(stderr.decode())


def privacy_AmplificationClient(key, keysize, K, M, N):
    # arguments: [key][  # bits] [algorithm] [endpoint:client/server] [host] [port] [user] [pw] [seq] [K] [M] [N]
    process = subprocess.Popen(
        [PA_path, key, str(keysize), "cellular", endpoint, host, port, user, pw, seq, str(K), str(M), str(N)],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    process.wait()
    if stderr.decode() != "":
        print(stderr.decode())
    lines = stdout.decode().split("\n")
    clientpatime = 0
    amplifiedkey = ""
    for line in lines:
        if "Elapsed time:" in line:
            clientpatime = float(line.split(":")[1])
        elif "amplified key:" in line:
            amplifiedkey = line.split(":")[1]
    return clientpatime, amplifiedkey


def privacy_AmplificationServer(key, keysize):
    # arguments: [key][  # bits] [algorithm] [endpoint:client/server] [host] [port] [user] [pw] [seq]
    process = subprocess.Popen(
        [PA_path, key, str(keysize), "cellular", endpoint, host, port, user, pw, seq],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    process.wait()
    if stderr.decode() != "":
        print(stderr.decode())
    lines = stdout.decode().split("\n")
    amplifiedkey = ""
    for line in lines:
        if "amplified key:" in line:
            amplifiedkey = line.split(":")[1]
    return amplifiedkey


def check_Connection(host):
    try:
        connection = pika.BlockingConnection(
            pika.ConnectionParameters(host=host, port=port, credentials=pika.PlainCredentials(user, pw)))
        channel = connection.channel()
        channel.queue_declare(queue="serverQueue" + seq)
        channel.close()
        connection.close()
        return True
    except pika.exceptions.AMQPConnectionError:
        return False


while not check_Connection(host):
    connection_attempts += 1
    if connection_attempts == 10:
        break
    time.sleep(7.0)
if connection_attempts == 10:
    print("Unable to connect to rabbitmq server")
    exit(-1)
if compression not in [0.1, 0.2, 0.5]:
    print("compression must be: 0.1, 0.2 or 0.5")
    exit(-1)
print("Starting " + endpoint + " with keysize: " + keysize + " and variant: " + variant + " and qber: " + qber)
bar = progressbar.ProgressBar(maxval=runs, widgets=[progressbar.Bar('=', '[', ']'), ' ', progressbar.Percentage()])
bar.start()


def ClientRoutine():
    error_Correction("client")
    print("Error correction completed")
    data, filename = extract_data()
    os.remove(path + "/" + filename)
    M, N = calcParameters(compression, int(keysize), pa_blocks)
    time, key = privacy_AmplificationClient(data["correctKey"], int(keysize), pa_blocks, M, N)
    pa_Times.append(time)
    data.update({"amplifiedKey": key})
    return data


def ServerRoutine():
    error_Correction("server")
    print("Error correction completed")
    filename = "key.json"
    file = open(path + "/" + filename, "r")
    data = json.load(file)
    print("Server correct key:" + data["correctKey"] + "\nWith size: " + str(len(data["correctKey"])))
    amplifiedkey = privacy_AmplificationServer(data["correctKey"], int(keysize))
    print("Server amplified key:" + amplifiedkey + "\nWith size: " + str(len(amplifiedkey)))
    os.remove(path + "/" + filename)


for i in range(runs):
    if endpoint == "client":
        data = ClientRoutine()
    elif endpoint == "server":
        ServerRoutine()
    else:
        print("Wrong arguments")

    bar.update(i + 1)
    sys.stdout.flush()
bar.finish()


def manage_Data(data):
    # save all data in a file .json
    if runs > 1:
        # experiment mode
        filename = variant + "_" + str(keysize) + "_" + "{:.6f}".format(float(qber)) + ".json"
        file = open(path + "/" + filename, "w")
        json.dump(
            {"elapsed_real_time": sum(er_Times) / len(er_Times), "ask_parity_messages": sum(exchanges) / len(exchanges),
             "unrealistic_efficiency": sum(efficiencies) / len(efficiencies),
             "pa_time": sum(pa_Times) / len(pa_Times)}, file)
        file.close()
    else:
        # demo mode
        filename = variant + "_" + str(keysize) + "_" + "{:.6f}".format(float(qber)) + ".json"
        file = open(path + "/" + filename, "w")
        json.dump(
            {"elapsed_real_time": sum(er_Times) / len(er_Times), "ask_parity_messages": sum(exchanges) / len(exchanges),
             "unrealistic_efficiency": sum(efficiencies) / len(efficiencies),
             "pa_time": sum(pa_Times) / len(pa_Times), "Client received key": data["receivedKey"],
             "Client correct key": data["correctKey"], "Client amplified key": data["amplifiedKey"],
             "Remaining errors": data["remaining_bit_errors"]}, file)
        file.close()


if endpoint == "client":
    manage_Data(data)
connection = pika.BlockingConnection(
    pika.ConnectionParameters(host=host, port=port, credentials=pika.PlainCredentials(user, pw)))
channel = connection.channel()
# delete all queues

channel.queue_delete(queue="clientQueue" + seq)
channel.queue_delete(queue="initQueue" + seq)
channel.queue_delete(queue="serverQueue" + seq)
channel.queue_delete(queue="clientQueuePA" + seq)
channel.queue_delete(queue="serverQueuePA" + seq)
channel.close()
connection.close()

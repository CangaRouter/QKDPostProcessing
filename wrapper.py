import math
import sys
import os
import subprocess
import re
import pika
import progressbar
import time
import json
from PrivacyAmplification.server import start_server
from PrivacyAmplification.client import start_client
from print_graphs import print_graph

ER_path = "./ErrorCorrection/Build/Mycascade"
# ER_path = "./Mycascade"
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
for i in range(runs):
    if endpoint == "client":
        error_Correction("client")
        data, filename = extract_data()
        os.remove(path + "/" + filename)
        M, N = calcParameters(compression, int(keysize), pa_blocks)
        pa_Times.append(start_client(data["correctKey"], int(port), user, pw, host, M, N, int(keysize), seq))
    elif endpoint == "server":
        error_Correction("server")
        filename = "key.json"
        file = open(path + "/" + filename, "r")
        data = json.load(file)
        start_server(data["correctKey"], int(port), user, pw, host, seq)
        os.remove(filename)
    else:
        print("Wrong arguments")

    bar.update(i + 1)
    sys.stdout.flush()
bar.finish()
if endpoint == "client":
    # save all data in a file .json
    filename = variant + "_" + str(keysize) + "_" + "{:.6f}".format(float(qber)) + ".json"
    file = open(path + "/" + filename, "w")
    json.dump(
        {"elapsed_real_time": sum(er_Times) / len(er_Times), "ask_parity_messages": sum(exchanges) / len(exchanges),
         "unrealistic_efficiency": sum(efficiencies) / len(efficiencies),
         "pa_time": sum(pa_Times) / len(pa_Times)}, file)
    file.close()
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

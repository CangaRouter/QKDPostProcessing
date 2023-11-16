import math
import sys
import os
import subprocess
import re
import pika
import progressbar
import time
import json


def calcParameters(compression, keyLenght, blocks):
    M = math.ceil(keyLenght / blocks)
    N = math.ceil(M * compression)
    return M, N


def extract_data(variant, keysize, qber, path, data_dictionary):
    filename = variant + "_" + str(keysize) + "_" + "{:.6f}".format(float(qber)) + ".json"
    file = open(path + "/" + filename, "r")
    data = json.load(file)
    data_dictionary["er_Times"].append(float(data["elapsed_real_time"]))
    data_dictionary["exchanges"].append(int(data["ask_parity_messages"]))
    data_dictionary["efficiencies"].append(float(data["unrealistic_efficiency"]))
    return data, filename, data_dictionary


def error_CorrectionClient(ER_path, keysize, variant, qber, host, port, user, pw, seq):
    # arguments: [mode:network/local/many] [#bits] [algorithm] [endpoint:client/server] [noise] [host] [port] [user] [pw] [seq]
    print("Arguments: ", ER_path, "network", keysize, variant, "client", qber, host, port, user, pw, seq)
    process = subprocess.Popen(
        [ER_path, "network", str(keysize), variant, "client", "{:.6f}".format(float(qber)),
         host, port,
         user, pw, seq], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    process.wait()
    if stderr.decode() != "":
        print(stderr.decode())


def error_CorrectionServer(ER_path, keysize, variant, qber, host, port, user, pw, seq, randomKey, key):
    # [mode:network/local/many] [#bits] [algorithm] [endpoint:client/server] [noise] [host] [port] [user] [pw] [seq/runs] [seq/keyType] [key]
    print("Arguments: ", ER_path, "network", keysize, variant, "server", qber, host, port, user, pw, seq, randomKey,
          key)
    process = subprocess.Popen(
        [ER_path, "network", str(keysize), variant, "server", "{:.6f}".format(float(qber)),
         host, str(port),
         user, pw, str(seq), str(randomKey), str(key)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    process.wait()
    if stderr.decode() != "":
        print(stderr.decode())


def privacy_AmplificationClient(PA_path, key, keysize, host, port, user, pw, seq, K, M, N):
    # arguments: [key][  # bits] [algorithm] [endpoint:client/server] [host] [port] [user] [pw] [seq] [K] [M] [N]
    process = subprocess.Popen(
        [PA_path, key, str(keysize), "cellular", "client", host, port, user, pw, seq, str(K), str(M), str(N)],
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


def privacy_AmplificationServer(PA_path, key, keysize, host, port, user, pw, seq):
    # arguments: [key][  # bits] [algorithm] [endpoint:client/server] [host] [port] [user] [pw] [seq]
    process = subprocess.Popen(
        [PA_path, str(key), str(keysize), "cellular", "server", host, str(port), user, pw, str(seq)],
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


def check_Connection(host, user, pw, seq, port=5672):
    try:
        connection = pika.BlockingConnection(
            pika.ConnectionParameters(host=host, port=port, credentials=pika.PlainCredentials(user, pw)))
        channel = connection.channel()
        channel.queue_declare(queue="serverQueue" + str(seq))
        channel.close()
        connection.close()
        return True
    except pika.exceptions.AMQPConnectionError:
        return False


def startLoop(ER_path, PA_path, endpoint, keysize, variant, qber, host, port, user, pw, seq,
              compression, pa_blocks, runs, randomKey=True, key=0, ):
    connection_attempts = 0
    while not check_Connection(host, user, pw, seq, port):
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
    print("Starting " + endpoint + " with keysize: " + str(keysize) + " and variant: " + variant + " and qber: " + str(
        qber))
    bar = progressbar.ProgressBar(maxval=runs, widgets=[progressbar.Bar('=', '[', ']'), ' ', progressbar.Percentage()])
    bar.start()
    for i in range(runs):
        if endpoint == "client":
            data, data_dictionary = ClientRoutine(ER_path, PA_path, keysize, variant, qber, host, port, user, pw, seq,
                                                  compression, pa_blocks)
        elif endpoint == "server":
            ServerRoutine(ER_path, PA_path, keysize, variant, qber, host, port, user, pw, seq, randomKey, key)
        else:
            print("Wrong arguments")

        bar.update(i + 1)
        sys.stdout.flush()
    bar.finish()
    if endpoint == "client":
        data_dictionary.update({"compression": compression})
        data_dictionary.update({"pa_blocks": pa_blocks})
        data_dictionary.update({"keysize": keysize})
        data_dictionary.update({"qber": qber})
        data_dictionary.update({"variant": variant})
        manage_Data(data, data_dictionary)
    connection = pika.BlockingConnection(
        pika.ConnectionParameters(host=host, port=port, credentials=pika.PlainCredentials(user, pw)))
    channel = connection.channel()
    # delete all queues
    channel.queue_delete(queue="clientQueue" + str(seq))
    channel.queue_delete(queue="initQueue" + str(seq))
    channel.queue_delete(queue="serverQueue" + str(seq))
    channel.queue_delete(queue="clientQueuePA" + str(seq))
    channel.queue_delete(queue="serverQueuePA" + str(seq))
    channel.close()
    connection.close()


def ClientRoutine(ER_path, PA_path, keysize, variant, qber, host, port, user, pw, seq, compression,
                  pa_blocks):
    data_dictionary = {"er_Times": [], "exchanges": [], "efficiencies": [], "pa_Times": []}
    error_CorrectionClient(ER_path, keysize, variant, qber, host, port, user, pw, seq)
    print("Error correction completed")
    data, filename, data_dictionary = extract_data(variant, keysize, qber, ".", data_dictionary)
    os.remove("." + "/" + filename)
    M, N = calcParameters(compression, int(keysize), pa_blocks)
    time, key = privacy_AmplificationClient(PA_path, data["correctKey"], int(keysize), host, port, user, pw, seq,
                                            pa_blocks, M, N)
    data_dictionary["pa_Times"].append(time)
    data.update({"amplifiedKey": key})
    return data, data_dictionary


def ServerRoutine(ER_path, PA_path, keysize, variant, qber, host, port, user, pw, seq, randomKey, key):
    error_CorrectionServer(ER_path, keysize, variant, qber, host, port, user, pw, seq, randomKey, key)
    print("Error correction completed")
    filename = "key.json"
    file = open("." + "/" + filename, "r")
    data = json.load(file)
    print("Server correct key:" + data["correctKey"] + "\nWith size: " + str(len(data["correctKey"])))
    amplifiedkey = privacy_AmplificationServer(PA_path, data["correctKey"], int(keysize), host, port, user, pw, seq)
    print("Server amplified key:" + amplifiedkey + "\nWith size: " + str(len(amplifiedkey.strip())))
    os.remove("." + "/" + filename)


def manage_Data(data, data_dictionary):
    # save all data in a file .json
    if runs > 1:
        # experiment mode
        filename = data_dictionary["variant"] + "_" + str(data_dictionary["keysize"]) + "_" + "{:.6f}".format(
            float(data_dictionary["qber"])) + ".json"
        file = open("." + "/" + filename, "w")
        json.dump(
            {"elapsed_real_time": sum(data_dictionary["er_Times"]) / len(data_dictionary["er_Times"]),
             "ask_parity_messages": sum(data_dictionary["exchanges"]) / len(data_dictionary["exchanges"]),
             "unrealistic_efficiency": sum(data_dictionary["efficiencies"]) / len(data_dictionary["efficiencies"]),
             "pa_time": sum(data_dictionary["pa_Times"]) / len(data_dictionary["pa_Times"]),
             "compression": float(data_dictionary["compression"]), "keysize": int(data_dictionary["keysize"])}, file)
        file.close()
    else:
        # demo mode
        filename = data_dictionary["variant"] + "_" + str(data_dictionary["keysize"]) + "_" + "{:.6f}".format(
            float(data_dictionary["qber"])) + ".json"
        file = open("." + "/" + filename, "w")
        json.dump(
            {"elapsed_real_time": sum(data_dictionary["er_Times"]) / len(data_dictionary["er_Times"]),
             "ask_parity_messages": sum(data_dictionary["exchanges"]) / len(data_dictionary["exchanges"]),
             "unrealistic_efficiency": sum(data_dictionary["efficiencies"]) / len(data_dictionary["efficiencies"]),
             "pa_time": sum(data_dictionary["pa_Times"]) / len(data_dictionary["pa_Times"]),
             "Client received key": data["receivedKey"],
             "Client correct key": data["correctKey"], "Client amplified key": data["amplifiedKey"],
             "Remaining errors": data["remaining_bit_errors"], "compression": float(data_dictionary["compression"]),
             "keysize": int(data_dictionary["keysize"])}, file)
        file.close()


if __name__ == '__main__':
    ER_path = "./error_correction/Mycascade"
    PA_path = "./privacy_amplification/PrivacyAmplificationCPP"
    key = ""
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
    randomKey = sys.argv[13]
    if randomKey in ["false", "False", "FALSE", "0", "no", "No", "NO", "n", "N"]:
        key = sys.argv[14]
    startLoop(ER_path, PA_path, endpoint, keysize, variant, qber, host, port, user, pw, seq,
              compression, pa_blocks, runs, randomKey, key)

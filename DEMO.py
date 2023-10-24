from docker import from_env, APIClient
from compose.cli.command import get_project
import time
import subprocess
import json
from time import sleep
from flask import Flask, render_template, request, Response

project = get_project('.')

def print_to_console(message):
    console_output.append(message)
def createEnvironment(qber,nBits,port,man_port,ER,compression,blocks,runs,seq,split_key):
    with open(".env", "w") as environment:
        environment.write("TAG=v1.6\n")
        environment.write("PORT="+str(port)+"\n")
        environment.write("MAN_PORT="+str(man_port)+"\n")
        environment.write("USR=QKD\n")
        environment.write("PW=QKD\n")
        environment.write("QBER="+"{:.6f}".format(float(qber))+"\n")
        environment.write("NBITS="+str(nBits)+"\n")
        environment.write("ER_VARIANT="+ER.lower()+"\n")
        environment.write("COMPRESSION="+str(compression)+"\n")
        environment.write("NBLOCKS="+str(blocks)+"\n")
        environment.write("RUNS="+str(runs)+"\n")
        environment.write("HOST=rabbitmq\n")
        environment.write("SEQ="+str(seq)+"\n")
        environment.write("SPLIT_KEY=" + str(split_key) + "\n")
        environment.flush()
        environment.close()


def runExperiment(qber,compression,keysize,variant,pa_blocks,seq):
    createEnvironment(qber, keysize, 5672, 15672, variant, compression, pa_blocks, 1, seq, False)
    project = get_project('.')
    project.up()
    containerClient=project.containers(service_names=["client"])[0]
    containerClient.wait()
    filename=variant+"_"+str(keysize)+"_"+"{:.6f}".format(float(qber)) +".json"
    #get the .json files from the container
    process = subprocess.Popen(
        ["docker", "cp", "thesis_client_1:/Client/"+filename, "./ResultsDEMO"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    process.wait()
    if process.returncode != 0:
        print(stderr.decode())
    file = open("./ResultsDEMO" + "/" + filename, "r")
    data = json.load(file)
    print_to_console("Client received Key: " + data["Client received key"])
    print_to_console("Client corrected key: " + data["Client correct key"])
    print_to_console("Remaning Errors: " + str(data["Remaining errors"]))
    print_to_console("Client amplified key: " + data["Client amplified key"])
    project.stop()
    sleep(10)
    project.remove_stopped()

app = Flask(__name__)

variants = ["original", "biconf", "yanetal", "option3", "option4", "option7", "option8"]
compressions = [0.1, 0.2, 0.4, 0.5]
keySizes = [128, 256, 512,640, 1024, 2048, 4096, 8192, 10000, 100000, 1000000, 10000000]
variant = "original"
compression = 0.1
pa_blocks = 10
keySize = 1024
qber = 0.01
console_output = []



@app.route('/')
def home():
    return render_template('index.html', variants=variants, compressions=compressions, keySizes=keySizes)

@app.route('/set_variant', methods=['POST'])
def set_variant():
    data = request.get_json()
    choice = data['choice']
    variant = choice
    print_to_console(variant)
    return "ok"

@app.route('/set_compression', methods=['POST'])
def set_compression():
    data = request.get_json()
    choice = data['choice']
    compression = float(choice)
    print_to_console(compression)
    return "ok"

@app.route('/set_qber', methods=['POST'])
def set_qber():
    data = request.get_json()
    choice = data['choice']
    qber = float(choice)
    print_to_console(qber)
    return "ok"

@app.route('/set_blocks', methods=['POST'])
def set_blocks():
    data = request.get_json()
    choice = data['choice']
    pa_blocks = int(choice)
    print_to_console(pa_blocks)
    return "ok"

@app.route('/set_keysize', methods=['POST'])
def set_keysize():
    data = request.get_json()
    choice = data['choice']
    keySize = int(choice)
    print_to_console(keySize)
    return "ok"

@app.route('/start_simulation', methods=['POST'])
def start_simulation():
    runExperiment(qber, compression, keySize, variant, pa_blocks, 1)
    return "ok"

@app.route('/get_variable')
def get_variable():
    global my_variable
    return str(my_variable)

@app.route('/stream_console')
def stream_console():
    def event_stream():
        while True:
            if console_output:
                yield 'data: {}\n\n'.format(console_output.pop(0))

    return Response(event_stream(), mimetype='text/event-stream')

if __name__ == '__main__':
        app.run(debug=True,host='0.0.0.0')

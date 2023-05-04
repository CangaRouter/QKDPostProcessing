from docker import from_env, APIClient
from compose.cli.command import get_project
import time
import subprocess

project = get_project('.')

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


def runExperiment(qber,compression,keysize,variant,seq):
    createEnvironment(qber, keysize, 5672, 15672, variant, compression, 10, 1000, seq, False)
    project = get_project('.')
    project.up()
    containerClient=project.containers(service_names=["client"])[0]
    containerClient.wait()
    #get the .json files from the container
    process = subprocess.Popen(
        ["docker", "cp", "thesis_client_1:/Client/"+variant+"_"+str(keysize)+"_"+"{:.6f}".format(float(qber)) +".json", "./Results"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    process.wait()
    if process.returncode != 0:
        print(stderr.decode())
def cycle(current_qber,current_keylength,step,independent_variable,compression,variant,end):
    seq=0
    while independent_variable == "qber" and current_qber < end:
        runExperiment(current_qber, compression, current_keylength, variant,seq)
        current_qber += step
        seq+=1
        project.remove_stopped()
    while independent_variable == "keylenght" and current_keylength < end:
        current_keylength += step
        runExperiment(current_qber, compression, current_keylength, variant)
        seq+=1
        project.remove_stopped()


#set the parameters for the experiment
independent_variable = "qber"
variant = ["original", "biconf", "yanetal", "option3", "option4", "option7", "option8"]
compression = 0.1
keySizes = [128, 256, 512, 1024, 2048, 4096, 8192]
qbers = [0.001, 0.1]
runs= 10000
step=(qbers[1]-qbers[0])/runs
#run the experiment
for variant in variant:
    cycle(qbers[0], 1000, step, independent_variable, compression, variant, qbers[1])





def printGraphs():
    if independent_variable == "qber":
        print_graph(qbers, er_Times, "QBER", "Execution Time")
        print_graph(qbers, exchanges, "QBER", "Message Exchanges")
        print_graph(qbers, efficiencies, "QBER", "Efficiency")
    elif independent_variable == "keylenght":
        print_graph(er_Times, keySizes, "Key Size", "EC Execution Time")
        print_graph(exchanges, keySizes, "Key Size", "Message Exchanges")
        print_graph(efficiencies, keySizes, "Key Size", "Efficiency")
        print_graph(pa_Times, keySizes, "Key Size", "PA Execution Time")




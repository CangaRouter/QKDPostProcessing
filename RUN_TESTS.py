from docker import from_env, APIClient
from compose.cli.command import get_project
import time
import subprocess
import sys


def createEnvironment(qber, nBits, port, man_port, ER, compression, blocks, runs, seq, random_key, key):
    with open(".env", "w") as environment:
        environment.write("TAG=v1.6\n")
        environment.write("PORT=" + str(port) + "\n")
        environment.write("MAN_PORT=" + str(man_port) + "\n")
        environment.write("USR=QKD\n")
        environment.write("PW=QKD\n")
        environment.write("QBER=" + "{:.6f}".format(float(qber)) + "\n")
        environment.write("NBITS=" + str(nBits) + "\n")
        environment.write("ER_VARIANT=" + ER.lower() + "\n")
        environment.write("COMPRESSION=" + str(compression) + "\n")
        environment.write("NBLOCKS=" + str(blocks) + "\n")
        environment.write("RUNS=" + str(runs) + "\n")
        environment.write("HOST=rabbitmq\n")
        environment.write("SEQ=" + str(seq) + "\n")
        environment.write("RANDOM_KEY=" + str(random_key) + "\n")
        environment.write("KEY=" + str(key) + "\n")
        environment.flush()
        environment.close()


def runExperiment(qber, compression, keysize, variant, seq, blocks, port, man_port, runs, ):
    createEnvironment(qber, keysize, port, man_port, variant, compression, blocks, runs, seq, True, 0)
    project = get_project('.')
    project.up()
    containerClient = project.containers(service_names=["client"])[0]
    containerClient.wait()
    # get the .json files from the container
    dir_name = os.getcwd().split("/")[-1].lower()
    process = subprocess.Popen(
        ["docker", "cp",
         dir_name+"_client_1:/Client/" + variant + "_" + str(keysize) + "_" + "{:.6f}".format(
             float(qber)) + ".json",
         "./Results"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    process.wait()
    if process.returncode != 0:
        print(stderr.decode())
    project.remove_stopped()


def cycle(current_qber, current_keysize, step, independent_variable, compression, variant, end, blocks, port, man_port,
          runs):
    seq = 0
    while independent_variable == "qber" and current_qber < end:
        runExperiment(current_qber, compression, current_keysize, variant, seq, blocks, port, man_port, runs)
        current_qber += step
        seq += 1

    while independent_variable == "keylenght" and current_keysize < end:
        runExperiment(current_qber, compression, current_keysize, variant, seq, blocks, port, man_port, runs)
        current_keysize += step
        seq += 1


def runTests(independent_variable, variant, compression, qber, keysize, runs, blocks, port, man_port):
    if variant.lower() == "all":
        variants = ["original", "biconf", "yanetal", "option3", "option4", "option7", "option8"]
    else:
        variants = variant.lower().split(",")
    if independent_variable == "qber":
        step = (qber[1] - qber[0]) / runs
        end = qber[1]
        current_qber = qber[0]
        current_keysize = keysize
    else:
        step = (keysize[1] - keysize[0]) / runs
        end = keysize[1]
        current_keysize = keysize[0]
        current_qber = qber

    for variant in variants:
        cycle(current_qber, current_keysize, step, independent_variable, compression, variant, end, blocks, port,
              man_port, runs)
        project.stop()


if __name__ == '__main__':
    # set the parameters for the experiment
    # arguments: [independent variable] [variant] [compression] [qber/keysize] [keysize/qber] [paBlocks] [port] [man_port]  [runs]
    # example: python3 RUN_TESTS.py qber all 0.05 0.01-0.05 100 5672 15672 10
    # example: python3 RUN_TESTS.py keysize all 0.05 128-1024 0.1 5672 15672 10
    choice = sys.argv[1].lower()
    if (choice == "qber"):
        qber = [float(x) for x in sys.argv[4].split("-")]
        keysize = int(sys.argv[5])
    else:
        keysize = [int(x) for x in sys.argv[4].split("-")]
        qber = float(sys.argv[5])

    runTests(choice, sys.argv[2], float(sys.argv[3]), qber, keysize, int(sys.argv[9]), int(sys.argv[6]),
             int(sys.argv[7]), int(sys.argv[8]))

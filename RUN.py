from docker import from_env
from compose.cli.command import get_project
import time

def createEnvironment(qber,nBits,port,man_port,variable,ER,compression,blocks):
    with open(".env", "w") as environment:
        environment.write("TAG=v1.6\n")
        environment.write("PORT="+str(port)+"\n")
        environment.write("MAN_PORT="+str(man_port)+"\n")
        environment.write("USR=QKD\n")
        environment.write("PW=QKD\n")
        environment.write("QBER="+str(qber)+"\n")
        environment.write("NBITS="+str(nBits)+"\n")
        environment.write("ER_VARIANT="+ER.lower()+"\n")
        environment.write("VARIABLE="+variable.lower()+"\n")
        environment.write("COMPRESSION="+str(compression)+"\n")
        environment.write("NBLOCKS="+str(blocks)+"\n")
        environment.close()


# Load the Docker environment
client = from_env()

createEnvironment(0.6,100,5672,15672,"qber","original",0.5,100)


# Load the Docker Compose project from the current directory
project = get_project('.')

# Start the Docker Compose project
project.up()



# Stop the Docker Compose project
#project.down(2,True)

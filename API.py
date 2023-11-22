from  wrapper import startLoop
from RUN_TESTS import runTests
def runPostProcessing(endpoint,keysize,variant,qber,host,port,user,pw,seq,randomKey,key,compression,pa_blocks,runs,ER_path="",PA_path=""):
    if ER_path!= "" and PA_path!="":
        startLoop(endpoint,keysize,variant,qber,host,port,user,pw,seq,randomKey,key,compression,pa_blocks,runs,ER_path,PA_path)
    else:
        startLoop(endpoint,keysize,variant,qber,host,port,user,pw,seq,randomKey,key,compression,pa_blocks,runs)
def runMultipleTests(independent_variable, variant, compression, qber, keysize, runs):
    runTests(independent_variable, variant, compression, qber, keysize, runs)

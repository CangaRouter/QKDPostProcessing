from  wrapper import startLoop
from RUN_TESTS import runTests
def runPostProcessing(ER_path,PA_path,endpoint,keysize,variant,qber,host,port,user,pw,seq,randomKey,key,compression,pa_blocks,runs):
    startLoop(ER_path,PA_path,endpoint,keysize,variant,qber,host,port,user,pw,seq,randomKey,key,compression,pa_blocks,runs)
def runMultipleTests(independent_variable, variant, compression, qber, keysize, runs):
    runTests(independent_variable, variant, compression, qber, keysize, runs)

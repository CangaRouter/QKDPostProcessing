 #include "mock_classical_session.h"
#include "debug.h"
#include "shuffled_key.h"

using namespace Cascade;

MockClassicalSession::MockClassicalSession( Server server) :
         fakeServer(server) {

}

Key MockClassicalSession::initialization() {
    Key noisyKey(fakeServer.getCorrectKey());
    noisyKey.apply_noise(fakeServer.getNoise());
    return noisyKey;
}

MockClassicalSession::~MockClassicalSession() = default;

void MockClassicalSession::start_iteration_with_shuffle_seed(int iteration_nr,
                                                             uint32_t shuffle_seed) {
    fakeServer.start_iteration_with_shuffle_seed(iteration_nr, std::to_string(shuffle_seed));
}

Key MockClassicalSession::closeConnection() {
    return fakeServer.getCorrectKey();
}

int* MockClassicalSession::channel_correct_parities(int* iterationNr, int* startBit, int* endBit, int nBlocks) {
    // Once we implement the real classical session, we will need to keep track of the blocks
    // for which we asked Alice the correct parity, but for which we have not yet received the
    // answer from Alice. For now, assume we get the answer immediately.
    int* parities=new int[nBlocks];
    for(int i=0;i<nBlocks;i++){
        parities[i]=fakeServer.return_correct_parities(iterationNr[i],startBit[i],endBit[i]);
    }
    return parities;
}



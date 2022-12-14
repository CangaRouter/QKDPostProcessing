#include "mock_classical_session.h"
#include "debug.h"
#include "shuffled_key.h"

using namespace Cascade;

MockClassicalSession::MockClassicalSession( Server server) :
         server(server) {

}

MockClassicalSession::~MockClassicalSession() = default;

void MockClassicalSession::start_iteration_with_shuffle_seed(int iteration_nr,
                                                             uint32_t shuffle_seed) {
    server.start_iteration_with_shuffle_seed(iteration_nr, std::to_string(shuffle_seed));
}



int MockClassicalSession::channel_correct_parities(int iterationNr, int startBit, int endBit) {
    // Once we implement the real classical session, we will need to keep track of the blocks
    // for which we asked Alice the correct parity, but for which we have not yet received the
    // answer from Alice. For now, assume we get the answer immediately.
    return server.return_correct_parities(iterationNr,startBit,endBit);
}



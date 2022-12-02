#include "mock_classical_session.h"
#include "debug.h"
#include "iteration.h"
#include "key.h"
#include "shuffle.h"
#include "shuffled_key.h"
#include "AMQPcpp.h"

using namespace Cascade;

MockClassicalSession::MockClassicalSession(Key &correct_key, bool cache_shuffles, Server server) :
        correct_key(correct_key),
        cache_shuffles(cache_shuffles), server(server) {

    DEBUG("Create MockClassicalSession: correct_key=" << correct_key.to_string());
}

MockClassicalSession::~MockClassicalSession() {
}

void MockClassicalSession::start_iteration_with_shuffle_seed(int iteration_nr,
                                                             uint64_t shuffle_seed) {
    int nr_key_bits = correct_key.get_nr_bits();
    ShufflePtr shuffle;
    shuffle = Shuffle::new_shuffle_from_seed(iteration_nr, nr_key_bits, shuffle_seed,
                                             cache_shuffles);
    ShuffledKeyPtr shuffled_key(new ShuffledKey(correct_key, shuffle));
    shuffled_keys[iteration_nr] = shuffled_key;
}

void MockClassicalSession::start_iteration_with_shuffle(int iteration_nr, ShufflePtr shuffle) {
    ShuffledKeyPtr shuffled_key(new ShuffledKey(correct_key, shuffle));
    shuffled_keys[iteration_nr] = shuffled_key;
}

void MockClassicalSession::ask_correct_parities(PendingItemQueue &ask_correct_parity_blocks) {
    // Once we implement the real classical session, we will need to keep track of the blocks
    // for which we asked Alice the correct parity, but for which we have not yet received the
    // answer from Alice. For now, assume we get the answer immediately.
    server.ask_correct_parities(ask_correct_parity_blocks);
}

void MockClassicalSession::test (int deltas){
    try {
//		AMQP amqp;
//		AMQP amqp(AMQPDEBUG);

        AMQP amqp("localhost:5672");		// all connect string

        AMQPExchange * ex = amqp.createExchange("e");
        ex->Declare("e", "fanout");

        AMQPQueue * qu2 = amqp.createQueue("q2");
        qu2->Declare();
        qu2->Bind( "e", "");

        std::string ss = "Differencies:  " + std::to_string(deltas);
        /* test very long message
        ss = ss+ss+ss+ss+ss+ss+ss;
        ss += ss+ss+ss+ss+ss+ss+ss;
        ss += ss+ss+ss+ss+ss+ss+ss;
        ss += ss+ss+ss+ss+ss+ss+ss;
        ss += ss+ss+ss+ss+ss+ss+ss;
*/

        ex->setHeader("Delivery-mode", 2);
        ex->setHeader("Content-type", "text/text");
        ex->setHeader("Content-encoding", "UTF-8");

        ex->Publish(  ss , ""); // publish very long message

        //ex->Publish(  "message 2 " , "");
      //  ex->Publish(  "message 3 " , "");


      /*  if (argc==2) {
            AMQPQueue * qu = amqp.createQueue();
            qu->Cancel(   amqp_cstring_bytes(argv[1]) );
        }*/

    } catch (AMQPException e) {
        std::cout << e.getMessage() << std::endl;
    }

}

#include "classical_session.h"
#include "debug.h"
#include "iteration.h"
#include "key.h"
#include "shuffle.h"
#include "shuffled_key.h"
#include "AMQPcpp.h"
#include <iostream>

using namespace Cascade;

ClassicalSession::ClassicalSession(Key &correct_key, bool cache_shuffles) :
        correct_key(correct_key),
        cache_shuffles(cache_shuffles) {
    DEBUG("Create ClassicalSession: correct_key=" << correct_key.to_string());
}

ClassicalSession::ClassicalSession(Key &correct_key, bool cache_shuffles, std::string hostip, std::string hostport, std::string exchange, std::string queue ) :
        correct_key(correct_key),
        cache_shuffles(cache_shuffles),
        hostip(hostip),
        hostport(hostport),
        exchange(exchange),
        queue(queue)
        {
    DEBUG("Create ClassicalSession: correct_key=" << correct_key.to_string());
}

ClassicalSession::~ClassicalSession() {
}

void
ClassicalSession::configureChannel(std::string hostip, std::string hostport, std::string exchange, std::string queue) {
    this->hostip = hostip;
    this->hostport = hostport;
    this->exchange = exchange;
    this->queue = queue;


}

void ClassicalSession::start_iteration_with_shuffle_seed(int iteration_nr,
                                                         uint64_t shuffle_seed) {
    int nr_key_bits = correct_key.get_nr_bits();
    ShufflePtr shuffle;
    shuffle = Shuffle::new_shuffle_from_seed(iteration_nr, nr_key_bits, shuffle_seed,
                                             cache_shuffles);
    ShuffledKeyPtr shuffled_key(new ShuffledKey(correct_key, shuffle));
    shuffled_keys[iteration_nr] = shuffled_key;
}

void ClassicalSession::start_iteration_with_shuffle(int iteration_nr, ShufflePtr shuffle) {
    ShuffledKeyPtr shuffled_key(new ShuffledKey(correct_key, shuffle));
    shuffled_keys[iteration_nr] = shuffled_key;
}

int onMessageReceiver( AMQPMessage * message  ) {
    uint32_t j = 0;
    const char * data = message->getMessage(&j);

    std::string buffer(data);

    buffer=buffer.substr(buffer.find(','));

    return stoi(buffer);
};

int onMessageSender( AMQPMessage * message  ) {
    int find=0;
    int arguments[4];
    uint32_t j = 0;
    const char * data = message->getMessage(&j);

    std::string buffer(data);
    for(int i=0;i<4;i++){
        arguments[i]=stoi(buffer.substr(find,buffer.find(',')));

    }

    buffer=buffer.substr(buffer.find(','));



    ShuffledKeyPtr shuffled_key = shuffled_keys[iteration_nr];
    int correctbit= shuffled_key->compute_range_parity(start_bit,
                                              end_bit);


    return 1;
};

void ClassicalSession::ask_correct_parities(PendingItemQueue &ask_correct_parity_blocks) {
    // Once we implement the real classical session, we will need to keep track of the blocks
    // for which we asked Alice the correct parity, but for which we have not yet received the
    // answer from Alice. For now, assume we get the answer immediately.
    for (auto it = ask_correct_parity_blocks.begin(); it != ask_correct_parity_blocks.end(); ++it) {
        PendingItem pending_item(*it);
        BlockPtr block = pending_item.block;
        int iteration_nr = block->get_iteration().get_iteration_nr();
        ShuffledKeyPtr shuffled_key = shuffled_keys[iteration_nr];
        int startBit=block->get_start_bit_nr();
        int endBit=block->get_end_bit_nr();
        int correct_parity = shuffled_key->compute_range_parity(block->get_start_bit_nr(),
                                                                block->get_end_bit_nr());
        block->set_correct_parity(correct_parity);

        DEBUG("Ask correct parity:" <<
                                    " block=" << block->debug_str() <<
                                    " correct_parity=" << correct_parity);
    }
}
int ClassicalSession::parityRound(int startBit, int endBit, int iteration){

    AMQP amqp(hostip+":"+hostport);
    AMQPExchange * ex=amqp.createExchange(exchange+"sender");
    ex->Declare("e", "fanout");
    AMQPQueue * qu2=amqp.createQueue(queue+"sender");
    qu2->Declare();
    qu2->Bind( exchange, "");
    ex->setHeader("Delivery-mode", 2);
    ex->setHeader("Content-type", "text/text");
    ex->setHeader("Content-encoding", "UTF-8");

    std::string message=std::to_string(seq++)+","+std::to_string(iteration)+","+std::to_string(startBit)+","+std::to_string(endBit);

    ex->Publish(message,"");

    AMQPQueue * qu1=amqp.createQueue(queue+"receiver");
    qu1->Declare();
    qu1->Bind(exchange+"receiver","");
    qu1->setConsumerTag("Received");
    qu1->addEvent(AMQP_MESSAGE, onMessage );

    qu1->Consume(AMQP_NOACK);

    return 1;

}

void ClassicalSession::openSenderChannel(){
    AMQP amqp(hostip+":"+hostport);
    AMQPQueue * qu1=amqp.createQueue(queue+"receiver");
    qu1->Declare();
    qu1->Bind(exchange+"receiver","");
    qu1->setConsumerTag("Received");
    qu1->addEvent(AMQP_MESSAGE, onMessageSender );

    qu1->Consume(AMQP_NOACK);
}

int ClassicalSession::reply_correct_parities(int iteration_nr, int start_bit, int end_bit) {

        ShuffledKeyPtr shuffled_key = shuffled_keys[iteration_nr];
        return shuffled_key->compute_range_parity(start_bit,
                                                                end_bit);
        DEBUG("Ask correct parity:" <<
                                    " block=" << block->debug_str() <<
                                    " correct_parity=" << correct_parity);


}

void ClassicalSession::test(int deltas) {
    try {


        AMQP amqp(hostip+":"+hostport);		// all connect string

        AMQPExchange * ex = amqp.createExchange(exchange);
        ex->Declare(exchange, "fanout");

        AMQPQueue * qu2 = amqp.createQueue(queue);
        qu2->Declare();
        qu2->Bind( exchange, "");

        std::string ss = "Differencies:  " + std::to_string(deltas);


        ex->setHeader("Delivery-mode", 2);
        ex->setHeader("Content-type", "text/text");
        ex->setHeader("Content-encoding", "UTF-8");

        ex->Publish(  ss , ""); // publish very long message


    } catch (AMQPException e) {
        std::cout << e.getMessage() << std::endl;
    }

}

Key &ClassicalSession::getCorrectKey() const {
    return correct_key;
}

void ClassicalSession::setCorrectKey(Key &correctKey) {
    correct_key = correctKey;
}

bool ClassicalSession::isCacheShuffles() const {
    return cache_shuffles;
}

void ClassicalSession::setCacheShuffles(bool cacheShuffles) {
    cache_shuffles = cacheShuffles;
}

const std::map<int, ShuffledKeyPtr> &ClassicalSession::getShuffledKeys() const {
    return shuffled_keys;
}

void ClassicalSession::setShuffledKeys(const std::map<int, ShuffledKeyPtr> &shuffledKeys) {
    shuffled_keys = shuffledKeys;
}

const std::string &ClassicalSession::getHostip() const {
    return hostip;
}

void ClassicalSession::setHostip(const std::string &hostip) {
    ClassicalSession::hostip = hostip;
}

const std::string &ClassicalSession::getHostport() const {
    return hostport;
}

void ClassicalSession::setHostport(const std::string &hostport) {
    ClassicalSession::hostport = hostport;
}

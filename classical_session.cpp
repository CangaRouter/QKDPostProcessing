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

/* @TODO move this method correctly to receiver. Why does it need the correct key?
 *  method moved to receiver
 * void ClassicalSession::start_iteration_with_shuffle_seed(int iteration_nr,
                                                         uint64_t shuffle_seed) {
    int nr_key_bits = correct_key.get_nr_bits();
    ShufflePtr shuffle;
    shuffle = Shuffle::new_shuffle_from_seed(iteration_nr, nr_key_bits, shuffle_seed,
                                             cache_shuffles);
    ShuffledKeyPtr shuffled_key(new ShuffledKey(correct_key, shuffle));
    shuffled_keys[iteration_nr] = shuffled_key;
}*/

void ClassicalSession::start_iteration_with_shuffle(int iteration_nr, ShufflePtr shuffle) {
    ShuffledKeyPtr shuffled_key(new ShuffledKey(correct_key, shuffle));
    shuffled_keys[iteration_nr] = shuffled_key;
}

/*int ClassicalSession::onMessageReceiver( AMQPMessage * message  ) {
    uint32_t j = 0;
    const char * data = message->getMessage(&j);

    std::string buffer(data);

    int seq1=stoi(buffer.substr(buffer.find(',')));
    buffer=buffer.substr (buffer.find(',')+1, buffer.length());

    receiverbit=stoi(buffer);
    return 1;
};*/

  /*int ClassicalSession::onMessageSender( AMQPMessage * message  ) {
    int find=0;
    int arguments[4];
    uint32_t j = 0;
    const char * data = message->getMessage(&j);

    std::string buffer(data);
    //Received message will be an array of comma-divided numbers so to parse it is just necessary to delete commas and retrieve values
    //following the format #sequence, #iteration, #startBit, #endBit
    for (auto& a :arguments)
    {
       a= stoi (buffer.substr (0, buffer.find (',')));
        buffer = buffer.substr (buffer.find (',')+1);
    }





    ShuffledKeyPtr shuffled_key = shuffled_keys[arguments[1]];
    int correctbit= shuffled_key->compute_range_parity(arguments[2],
                                              arguments[3]);
    AMQP amqp(hostip+":"+hostport);

    AMQPExchange * correctEx=amqp.createExchange(exchange+"sender");
    correctEx->Declare("e", "fanout");
    AMQPQueue * correctQueue=amqp.createQueue(queue+"sender");
    correctQueue->Declare();
    correctQueue->Bind( exchange, "");
    correctEx->setHeader("Delivery-mode", 2);
    correctEx->setHeader("Content-type", "text/text");
    correctEx->setHeader("Content-encoding", "UTF-8");

    std::string correctMessage= arguments[0]+","+  std::to_string(correctbit);

    correctEx->Publish(correctMessage,"");


    return 1;
};*/

void ClassicalSession::ask_correct_parities(PendingItemQueue &ask_correct_parity_blocks) {
    // Once we implement the real classical session, we will need to keep track of the blocks
    // for which we asked Alice the correct parity, but for which we have not yet received the
    // answer from Alice. For now, assume we get the answer immediately.
    for (auto it = ask_correct_parity_blocks.begin(); it != ask_correct_parity_blocks.end(); ++it) {
        PendingItem pending_item(*it);
        BlockPtr block = pending_item.block;
        int iteration_nr = block->get_iteration().get_iteration_nr();
        ShuffledKeyPtr shuffled_key = shuffled_keys[iteration_nr];
        int correct_parity = parityRound(block->get_start_bit_nr(),
                                                                block->get_end_bit_nr(),iteration_nr);
        block->set_correct_parity(correct_parity);
        senderbit=-1;

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

    //following the format #sequence, #iteration, #startBit, #endBit

    std::string message=std::to_string(seq++)+","+std::to_string(iteration)+","+std::to_string(startBit)+","+std::to_string(endBit);

    ex->Publish(message,"");

    AMQPQueue * qu1=amqp.createQueue(queue+"receiver");
    qu1->Declare();
    qu1->Bind(exchange+"receiver","");
    qu1->setConsumerTag("Received");
   // qu1->addEvent(AMQP_MESSAGE, onMessageReceiver );
    qu1->Consume(AMQP_NOACK);

     if(receiverbit!=0 && receiverbit !=1)
         throw "suca";

    return receiverbit;

}

void ClassicalSession::openSenderChannel(){
    AMQP amqp(hostip+":"+hostport);
    AMQPQueue * qu1=amqp.createQueue(queue+"receiver");
    qu1->Declare();
    qu1->Bind(exchange+"receiver","");
    qu1->setConsumerTag("Received");
  //  qu1->addEvent(AMQP_MESSAGE, onMessageSender );
    qu1->Consume(AMQP_NOACK);
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

//
// Created by Marco Pappalardo on 21/07/2022.
//

#include "RabbitmqClassicalSession.h"

#include <utility>
#include "AMQPcpp.h"


using namespace Cascade;

RabbitmqClassicalSession::RabbitmqClassicalSession(std::string queue,
                                                   std::string host, std::string port) : queue(std::move(queue)),
                                                                                                       exchange(std::move(
                                                                                                               exchange)),
                                                                                                       host(std::move(host)),
                                                                                                       port(std::move(port)) {}

RabbitmqClassicalSession::~RabbitmqClassicalSession() {

}

void RabbitmqClassicalSession::start_iteration_with_shuffle_seed(int iteration_nr, uint32_t shuffle_seed) {
    AMQP amqp("localhost:5672");



    AMQPExchange * client = amqp.createExchange("client");
    client->Declare("client","direct");
    AMQPQueue * replyQueue = amqp.createQueue("reply");
    AMQPQueue * requestQueue= amqp.createQueue("request");
    replyQueue->Declare();
    requestQueue->Declare();
    requestQueue->Bind("client","request");

    client->setHeader("correlation_id","ghei" );
    client->setHeader("Reply-to","reply");
    client->setHeader("Delivery-mode",AMQP_DELIVERY_PERSISTENT);
    client->setHeader("Content-type", "text/text");
    client->setHeader("Content-encoding", "UTF-8");

    client->Publish( "reply pls" , "request");

    replyQueue->setConsumerTag("tag_RPC");
    replyQueue->addEvent(AMQP_MESSAGE, onMessage );

    replyQueue->Consume(AMQP_NOACK);








}

int RabbitmqClassicalSession::channel_correct_parities(int iterationNr, int startBit, int endBit) {

    AMQP amqp(host+":"+port);


     AMQPExchange * client = amqp.createExchange(exchange);
    AMQPQueue * replyQueue = amqp.createQueue("");
    client->Declare(exchange,"direct");
    replyQueue->Declare();
    replyQueue->Bind(exchange,"reply");
    AMQPQueue * requestQueue= amqp.createQueue(queue);
    requestQueue->Declare();
    replyQueue->Bind(exchange,"request");
    int correlation=9;

    client->setHeader("correlationID",correlation );
    client->setHeader("Delivery-mode", AMQP_DELIVERY_PERSISTENT);
    client->setHeader("Content-type", "text/text");
    client->setHeader("Content-encoding", "UTF-8");


    return 0;
}

void RabbitmqClassicalSession::test (int deltas){
    try {
        //		AMQP amqp;
        //		AMQP amqp(AMQPDEBUG);

        AMQP amqp(host+":"+port);		// all connect string

        AMQPExchange * ex = amqp.createExchange(exchange);
        ex->Declare(exchange, "fanout");

        AMQPQueue * qu2 = amqp.createQueue(queue);
        qu2->Declare();
        qu2->Bind( exchange, "");

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

        ex->
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



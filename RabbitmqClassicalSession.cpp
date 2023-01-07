//
// Created by Marco Pappalardo on 21/07/2022.
//

#include "RabbitmqClassicalSession.h"

#include <utility>
#include <amqpcpp.h>
#include <sstream>


using namespace Cascade;

RabbitmqClassicalSession::RabbitmqClassicalSession(std::string host, int port, std::string user, std::string pw)
: host(host), port(port), user(user), pw(pw)
{
    connection = new AMQP::TcpConnection(handler,
                                          AMQP::Address(host, port, AMQP::Login(user, pw), "/"));
    channel = new AMQP::TcpChannel(connection);
    channel->onError([=](const char* message)
                      {
                          std::cout << "Channel error: " << message << std::endl;
                          handler.Stop();
                      });
    channel->setQos(1);
    channel->declareQueue("clientQueue");


}

RabbitmqClassicalSession::~RabbitmqClassicalSession() {

}


void RabbitmqClassicalSession::closeConnection() {
    channel->cancel("alfredo");
    connection->close();
}

void RabbitmqClassicalSession::start_iteration_with_shuffle_seed(int iteration_nr, uint32_t shuffle_seed) {
    AMQP::Table headers;
    std::string body="cascade";
    headers.set("startIterationWithSeed",std::to_string(shuffle_seed));
    headers.set("nIteration",iteration_nr);

    AMQP::Envelope env(body.c_str(),body.size());
    env.setHeaders(headers);
    channel->publish("","serverQueue",env);

}

int RabbitmqClassicalSession::channel_correct_parities(int iterationNr, int startBit, int endBit) {
    std::string correlationId=std::to_string(iterationNr)+","+std::to_string(startBit)+","+std::to_string(endBit) ;
    AMQP::Table headers;
    headers.set("startBit",startBit);
    headers.set("endBit",endBit);
    headers.set("nIteration",iterationNr);
    AMQP::Envelope env("",0);
    env.setCorrelationID(correlationId);
    env.setReplyTo("clientQueue");
    env.setHeaders(headers);
    channel->publish("", "serverQueue", env);
    int response = -1;
    channel->consume("clientQueue",AMQP::noack)
            .onReceived([&](const AMQP::Message& m, uint64_t, bool)
                        {
                            if (m.correlationID() != correlationId)
                                return; // just skip it

                            response = m.headers().get("parity");
                            handler.Stop();
                        });
    handler.Start();
    return response;
}






//
// Created by marco on 27/06/23.
//

#include <string>
#include "RabbitmqClassicalSession.h"

RabbitmqClassicalSession::RabbitmqClassicalSession(std::string host, int port, std::string user, std::string pw,
                                                   std::string seq)
        : host(host), port(port), user(user), pw(pw), id(seq) {
    connection = new AMQP::TcpConnection(handler,
                                         AMQP::Address(host, port, AMQP::Login(user, pw), "/"));
    channel = new AMQP::TcpChannel(connection);
    channel->onError([=, this](const char *message) {
        std::cout << "Channel error: " << message << std::endl;
        handler.Stop();
    });
    channel->setQos(10);
    channel->declareQueue("clientQueuePA" + id);
    channel->declareQueue("serverQueuePA" + id);
    tag = std::to_string(rand());
}
RabbitmqClassicalSession::~RabbitmqClassicalSession() {

}



CellularAutomataConfig RabbitmqClassicalSession::NegotiateServer(){
    CellularAutomataConfig config;
    std::vector<std::string> responseStr;
    channel->consume("serverQueuePA" + id, tag + "_server", AMQP::noack)
            .onReceived([&](const AMQP::Message &m, uint64_t tag, bool) {
                const AMQP::Table &headers = m.headers();
                config.K = headers.get("K");
                config.M = headers.get("M");
                config.N = headers.get("N");
                boost::split(responseStr, m.body(), boost::is_any_of("\n"));
                for (int k = 0; k < (config.N/64+(config.N%64? 1:0)); k++) {
                    try {
                        config.initial_value.push_back(stoull(responseStr[k], nullptr, 10));
                    } catch (...) {
                        std::cerr << "RabbitmqClassicalSession_PAServer "<<"Max: "<< (config.N/64+(config.N%64? 1:0)) << " Actual: " << k << std::endl;
                        std::cerr << m.body() << std::endl;
                        exit(-1);
                    }
                }
                correlationId=m.correlationID();
                replyTo=m.replyTo();
                channel->cancel(this->tag + "_server");
                handler.Stop();
            });
    handler.Start();
    return config ; //CellularAutomataConfig(nBlocks, response);
}

void RabbitmqClassicalSession::NegotiateClient(int K, int M, int N, std::vector<uint64_t> initial_value) {
    CellularAutomataConfig config;
    correlationId = std::to_string(rand());
    AMQP::Table headers;
    headers.set("messageType", "PaNegotiaion");
    headers.set("K", K);
    headers.set("M", M);
    headers.set("N", N);
    std::string body;
    for (auto &it: initial_value) {
        body += std::to_string(it) + '\n';
    }
    AMQP::Envelope env(body.c_str());
    env.setCorrelationID(correlationId);
    env.setReplyTo("clientQueuePA" + id);
    env.setHeaders(headers);
    channel->startTransaction();
// publish a number of messages
    channel->publish("", "serverQueuePA" + id, env);
    // commit the transactions, and set up callbacks that are called when
// the transaction was successful or not
    channel->commitTransaction()
            .onSuccess([&]() {
              handler.Stop();
                // all messages were successfully published
            })
            .onError([](const char *message) {
                // none of the messages were published
                // now we have to do it all over again
            });
    handler.Start();
    //CellularAutomataConfig(nBlocks, response);

}

void RabbitmqClassicalSession::CloseClient() {

    channel->consume("clientQueuePA" + id, tag + "_ClientClose", AMQP::noack)
            .onReceived([&](const AMQP::Message &m, uint64_t tag, bool) {
                if (m.correlationID() != correlationId)
                    return; // just skip it
                const AMQP::Table &headers = m.headers();
                std::string messageType = headers.get("messageType");
                if (messageType != "closingConfirm") {
                    std::cerr << "RabbitmqClassicalSession_CloseClient" << std::endl;
                    std::cerr << m.body() << std::endl;
                    exit(-1);
                }

                channel->cancel(this->tag + "_ClientClose");
                channel->close();
                connection->close();
            });
    handler.Start();

}
void RabbitmqClassicalSession::CloseServer() {
    AMQP::Envelope env("", 0);
    AMQP::Table headers;
    headers.set("messageType", "closingConfirm");
    env.setHeaders(headers);
    env.setCorrelationID(correlationId);
    channel->startTransaction();
// publish a number of messages
    channel->publish("",replyTo, env);
    // commit the transactions, and set up callbacks that are called when
// the transaction was successful or not
    channel->commitTransaction()
            .onSuccess([&]() {
                channel->cancel(this->tag + "_Server");
                channel->close();
                connection->close();
                // all messages were successfully published
            })
            .onError([](const char *message) {
                // none of the messages were published
                // now we have to do it all over again
            });
    handler.Start();
}
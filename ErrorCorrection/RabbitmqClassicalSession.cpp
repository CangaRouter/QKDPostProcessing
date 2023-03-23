//
// Created by Marco Pappalardo on 21/07/2022.
//

#include "RabbitmqClassicalSession.h"

#include <utility>
#include <amqpcpp.h>
#include <sstream>


using namespace Cascade;

RabbitmqClassicalSession::RabbitmqClassicalSession(std::string host, int port, std::string user, std::string pw)
        : host(host), port(port), user(user), pw(pw) {
    connection = new AMQP::TcpConnection(handler,
                                         AMQP::Address(host, port, AMQP::Login(user, pw), "/"));
    channel = new AMQP::TcpChannel(connection);
    channel->onError([=](const char *message) {
        std::cout << "Channel error: " << message << std::endl;
        handler.Stop();
    });
    channel->setQos(1);
    channel->declareQueue("clientQueue");
    channel->declareQueue("initQueue");
    tag = std::to_string(rand());
}

RabbitmqClassicalSession::~RabbitmqClassicalSession() {

}


Key RabbitmqClassicalSession::closeConnection() {
    std::string correlationId = std::to_string(rand());
    std::vector<std::string> responseStr;
    std::vector<uint64_t> words;
    int nrBits = 0;
    int nrWords = 0;
    AMQP::Table headers;
    headers.set("messageType", "closing");
    AMQP::Envelope env("", 0);
    env.setCorrelationID(correlationId);
    env.setReplyTo("clientQueue");
    env.setHeaders(headers);
    channel->publish("", "serverQueue", env);
    channel->consume("clientQueue", tag+"_close", AMQP::noack)
            .onReceived([&](const AMQP::Message &m, uint64_t, bool) {
                if (m.correlationID() != correlationId)
                    return; // just skip it
                AMQP::Table headers = m.headers();
                nrBits = headers.get("nrBits");
                nrWords = headers.get("nrWords");
                std::string bodyStr(m.body());
                boost::split(responseStr, m.body(), boost::is_any_of("\n"));
                responseStr.pop_back();
                for (auto &it: responseStr) {
                    words.push_back(stoull(it, nullptr, 10));
                }
                channel->removeQueue("clientQueue");
                handler.Stop();
            });
    handler.Start();
    connection->close();
    Key correctKey = Key(nrBits, nrWords, words);
    return correctKey;
}

Key RabbitmqClassicalSession::initialization() {
    std::vector<std::string> responseStr;
    std::vector<uint64_t> words;
    int nrBits = 0;
    int nrWords = 0;
    std::string correlationId = std::to_string(rand());
    AMQP::Table headers;
    headers.set("messageType", "initialization");
    AMQP::Envelope env("", 0);
    env.setCorrelationID(correlationId);
    env.setReplyTo("initQueue");
    env.setHeaders(headers);
    channel->publish("", "serverQueue", env);
    channel->consume("initQueue", tag+"_init", AMQP::noack)
            .onReceived([&](const AMQP::Message &m, uint64_t, bool) {
                if (m.correlationID() != correlationId)
                    return; // just skip it
                AMQP::Table headers = m.headers();
                nrBits = headers.get("nrBits");
                nrWords = headers.get("nrWords");
                std::string bodyStr(m.body());
                boost::split(responseStr, m.body(), boost::is_any_of("\n"));
                responseStr.pop_back();
                for (auto &it: responseStr) {
                    words.push_back(stoull(it, nullptr, 10));
                }
                channel->removeQueue("initQueue");
                handler.Stop();
            });
    handler.Start();
    Key noisyKey = Key(nrBits, nrWords, words);
    return noisyKey;

}

void RabbitmqClassicalSession::start_iteration_with_shuffle_seed(int iteration_nr, uint32_t shuffle_seed) {
    AMQP::Table headers;
    std::string body = "cascade";
    headers.set("seed", std::to_string(shuffle_seed));
    headers.set("nIteration", iteration_nr);
    headers.set("messageType", "startIteration");


    AMQP::Envelope env(body.c_str(), body.size());
    env.setHeaders(headers);
    channel->publish("", "serverQueue", env);

}

int *RabbitmqClassicalSession::channel_correct_parities(int *iterationNr, int *startBit, int *endBit, int nBlocks) {
    int *response = new int[nBlocks];
    std::vector<std::string> responseStr;
    std::string correlationId = std::to_string(rand());
    AMQP::Table headers;
    headers.set("numberOfBlocks", nBlocks);
    headers.set("messageType", "cascade_rpc");
    std::string body = "";
    for (int i = 0; i < nBlocks; i++) {
        body += std::to_string(iterationNr[i]) + "," + std::to_string(startBit[i]) + "," + std::to_string(endBit[i]) +
                "\n";
    }
    AMQP::Envelope env(body.c_str(), body.size());
    env.setCorrelationID(correlationId);
    env.setReplyTo("clientQueue");
    env.setHeaders(headers);
    channel->publish("", "serverQueue", env);

    channel->consume("clientQueue", tag+"_client", AMQP::noack)
            .onReceived([&](const AMQP::Message &m, uint64_t, bool) {
                if (m.correlationID() != correlationId)
                    return; // just skip it
                boost::split(responseStr, m.body(), boost::is_any_of(","));
                for (int i = 0; i < nBlocks; ++i) {
                    response[i] = stoi(responseStr[i]);
                }
                channel->cancel(tag+"_client");
                handler.Stop();
            });
    handler.Start();
    return response;
}






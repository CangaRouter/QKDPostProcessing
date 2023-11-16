//
// Created by marco on 27/06/23.
//

#ifndef PRIVACYAMPLIFICATIONCPP_RABBITMQCLASSICALSESSION_H
#define PRIVACYAMPLIFICATIONCPP_RABBITMQCLASSICALSESSION_H

#include <amqpcpp.h>
#include "ConnHandler.h"
#include "ClassicalSession.h"
#include <sstream>
#include <thread>
#include <boost/algorithm/string.hpp>

typedef struct {
    std::string host;
    int port;
    std::string user;
    std::string pw;
    std::string seq;
} RabbitmqConfig;



class RabbitmqClassicalSession : public ClassicalSession{
    public:
    RabbitmqClassicalSession(std::string host, int port, std::string user, std::string pw,std::string seq);
    ~RabbitmqClassicalSession() override;
    CellularAutomataConfig NegotiateServer() override;
    void NegotiateClient(int K, int M, int N, std::vector<uint64_t> initial_value) override;
    void CloseClient() override;
    void CloseServer() override;
private:
    std::string correlationId;
    std::string replyTo;
    std::string host;
    int port;
    std::string user;
    std::string pw;
    AMQP::TcpConnection* connection;
    ConnHandler handler;
    AMQP::TcpChannel* channel;
    std::string tag;
    std::string id;
};


#endif //PRIVACYAMPLIFICATIONCPP_RABBITMQCLASSICALSESSION_H

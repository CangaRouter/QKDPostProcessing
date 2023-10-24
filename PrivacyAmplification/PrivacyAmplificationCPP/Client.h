//
// Created by marco on 27/06/23.
//

#ifndef PRIVACYAMPLIFICATIONCPP_CLIENT_H
#define PRIVACYAMPLIFICATIONCPP_CLIENT_H

#include <string>
#include <iostream>
#include <vector>
#include "ClassicalSession.h"
#include "Key.h"
#include "CellularAutomataPA.h"
class Client {

public:
    Client ( ClassicalSession &classicalSession, const Key &key, std::string pw, std::string host,
            std::string user, std::string seq, int port, std::string mode, int size);

    double PrivacyAmplification(int K, int M, int N, std::vector<uint64_t> initial_value);


private:
    ClassicalSession &classical_session;

    Key key;
    std::string pw;
    std::string host;
    std::string user;
    std::string seq;
    int port;
    std::string mode;
    int size;
};


#endif //PRIVACYAMPLIFICATIONCPP_CLIENT_H

//
// Created by marco on 27/06/23.
//

#ifndef PRIVACYAMPLIFICATIONCPP_SERVER_H
#define PRIVACYAMPLIFICATIONCPP_SERVER_H
#include <string>
#include <iostream>
#include <vector>
#include "ClassicalSession.h"
#include "Key.h"
#include "CellularAutomataPA.h"

class Server {
public:
    Server( ClassicalSession &classicalSession, const Key &key, std::string pw, std::string host,
           std::string user, std::string seq, int port, std::string mode, int size);

    std::string PrivacyAmplification();


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


#endif //PRIVACYAMPLIFICATIONCPP_SERVER_H

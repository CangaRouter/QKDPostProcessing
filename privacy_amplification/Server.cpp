//
// Created by marco on 27/06/23.
//

#include "Server.h"

#include <utility>




Server::Server(ClassicalSession &classicalSession, const Key &key, std::string pw, std::string host,
               std::string user, std::string seq, int port, std::string mode, int size)
        : classical_session(classicalSession), key(key), pw(std::move(pw)), host(std::move(host)), user(std::move(user)), seq(std::move(seq)), port(port),
          mode(std::move(mode)), size(size) {}

std::string Server::PrivacyAmplification(){
    if(mode == "cellular"){
        CellularAutomataConfig config=classical_session.NegotiateServer();
        Key amplifiedKey= CellularAutomataPA::Amplificate(key,config.K,config.M,config.N,config.initial_value);
        classical_session.CloseServer();
        std::cout<<"amplified key: " <<amplifiedKey.to_string() <<std::endl;
        return amplifiedKey.to_string();
    }
    return "";
}



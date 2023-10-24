#include <iostream>
#include "Server.h"
#include "RabbitmqClassicalSession.h"
#include "Client.h"

int main(int argc, char** argv) {
    // arguments: [key] [#bits] [algorithm] [endpoint:client/server] [host] [port] [user] [pw] [seq] [K] [M] [N]
    std::string strKey(argv[1]);
    int size = atoi(argv[2]);
    std::string algorithmName(argv[3]);
    std::string endpoint(argv[4]);
    std::string host(argv[5]);
    int port = atoi(argv[6]);
    std::string user(argv[7]);
    std::string pw(argv[8]);
    std::string seq(argv[9]);
    Key key=Key::parseKey(strKey);
    RabbitmqClassicalSession classical_session(host, port, user, pw, seq);
    if(endpoint == "server"){
        Server server(classical_session, key,pw,host,user,seq,port,algorithmName,size);
        server.PrivacyAmplification();
        return 0;
    } else if(endpoint == "client"){
        int K = atoi(argv[10]);
        int M = atoi(argv[11]);
        int N = atoi(argv[12]);
        Client client(classical_session, key,pw,host,user,seq,port,algorithmName,size);
        double time=client.PrivacyAmplification(K,M,N,{0xffffffffff});
        std::cout<<"Elapsed time: "<<time<<std::endl;
    } else{
        std::cout<<"endpoint not recognized"<<std::endl;
    }
    return 0;
}

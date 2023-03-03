#include "algorithm.h"
#include "key.h"
#include "Client.h"
#include "Server.h"
#include "RabbitmqClassicalSession.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <thread>
#include <string.h>
#include <cstdlib>
#include <amqpcpp.h>
#include "conn_handler.h"


using namespace Cascade;
void runOnRabbitmq(char* algorithm_name, int size, char* endpoint, double noise,std::string host,int port,std::string user,std::string pw);
void runLocal(char* algorithm_name, int size, char* endpoint, double noise);
int main(int argc, char **argv) {
   if (argc != 10) {
        std::perror("Wrong number of arguments \n Usage ./Mycascade [mode:network/local] [#bits] [algorithm] [endpoint:client/server]  ");
        for (int j=0; j<argc;j++){
            std::perror(argv[j]);
        }
    }
    double noise = atof(argv[5]);
    int size =atoi(argv[2]);
    std::string host(argv[6]);
    int port=atoi(argv[7]);
    std::string user(argv[8]);
    std::string pw(argv[9]);

    if (strcmp(argv[1], "network") == 0) {

           ConnHandler handler;

           AMQP::TcpConnection *connection= new AMQP::TcpConnection(handler,
                                                                    AMQP::Address(host, port, AMQP::Login(user, pw),
                                                                                  "/"));
           AMQP::TcpChannel *channel= new AMQP::TcpChannel(connection);
           while(!channel->ready()) {
               sleep(7);
            connection   = new AMQP::TcpConnection(handler,
                                         AMQP::Address(host, port, AMQP::Login(user, pw),
                                                       "/"));
            channel = new AMQP::TcpChannel(connection);
            channel->onError([&](const char *message) {
                   std::cout << "Server offline, retrying in 7 seconds " << std::endl;
                   handler.Stop();
               });
               channel->onReady([&]() {
                   handler.Stop();
               });
               handler.Start();
           }
        runOnRabbitmq(argv[3], size, argv[4], noise, host, port, user, pw);

    } else if (strcmp(argv[1], "local") == 0)  {
        runLocal(argv[3],  size, argv[4], noise);
    }else
        std::cout<<"argument 4 must be either network or local"<<std::endl;
}


void runOnRabbitmq(char* algorithm_name, int size, char* endpoint, double noise,std::string host,int port,std::string user,std::string pw){

    const Algorithm *algorithm = Algorithm::get_by_name(algorithm_name);
    if (strcmp(endpoint, "server") == 0) {
            std::cout << "Running serverside network test..." << std::endl;
            Key correct_key = Cascade::Key(size);
            std::cout << "Server correct key: " << correct_key.to_string() << std::endl;
            Server server(correct_key, true, host, port, user, pw, noise);
            std::cout <<"Error correction completed"<<std::endl;
        return ;
    } else if (strcmp(endpoint, "client") == 0) {
        std::cout<<"Running clientside network test..."<<std::endl;
        RabbitmqClassicalSession classical_session(host, port, user, pw);
        Client client(*algorithm, classical_session,size,noise);
        client.reconcile();
        Key &reconciled_key = client.get_reconciled_key();
        std::cout << "Client correct key: " << reconciled_key.to_string();
        return ;
    } else {
        std::cout << "Argument 3 must be either 'sender' or 'receiver'" << std::endl;
    }

}


void runLocal(char* algorithm_name, int size, char* endpoint, double noise){
    std::cout<<"Running local test..."<<std::endl;
    const Algorithm *algorithm = Algorithm::get_by_name(algorithm_name);
    Key correct_key = Cascade::Key(size);
    MockClassicalSession classical_session(Server(correct_key, false));
    Client client(*algorithm, classical_session ,size,noise);
    client.reconcile();
    Key &reconciled_key = client.get_reconciled_key();
    std::cout << "Differencies: " << correct_key.nr_bits_different(reconciled_key);
    return ;
}

#include "algorithm.h"
#include "key.h"
#include "Client.h"
#include "Server.h"
#include "RabbitmqClassicalSession.h"
#include <iostream>
#include <thread>
#include <string.h>
#include <cstdlib>
#include <amqpcpp.h>
#include <fstream>
#include "conn_handler.h"
#include "data_point.h"


using namespace Cascade;

void runOnRabbitmq(char *algorithm_name, int size, char *endpoint, double noise, std::string host, int port,
                   std::string user, std::string pw);

void runLocal(char *algorithm_name, int size, double noise);

void printJsonClient(double errorRate, int nrBits, std::string algorithm, Stats stats);
void printJsonServer(std::string key, std::string tag);

int main(int argc, char **argv) {
    if (argc != 10) {
        std::perror(
                "Wrong number of arguments \n Usage ./Mycascade [mode:network/local] [#bits] [algorithm] [endpoint:client/server]  ");
        for (int j = 0; j < argc; j++) {
            std::perror(argv[j]);
        }
    }
    double noise = atof(argv[5]);
    int size = atoi(argv[2]);
    std::string host(argv[6]);
    int port = atoi(argv[7]);
    std::string user(argv[8]);
    std::string pw(argv[9]);

    if (strcmp(argv[1], "network") == 0) {

        ConnHandler handler;

        AMQP::TcpConnection *connection = new AMQP::TcpConnection(handler,
                                                                  AMQP::Address(host, port, AMQP::Login(user, pw),
                                                                                "/"));
        AMQP::TcpChannel *channel = new AMQP::TcpChannel(connection);
        while (!channel->ready()) {
            sleep(7);
            connection = new AMQP::TcpConnection(handler,
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

    } else if (strcmp(argv[1], "local") == 0) {
        runLocal(argv[3], size, noise);
    } else
        std::cout << "argument 4 must be either network or local" << std::endl;
}


void runOnRabbitmq(char *algorithm_name, int size, char *endpoint, double noise, std::string host, int port,
                   std::string user, std::string pw) {

    const Algorithm *algorithm = Algorithm::get_by_name(algorithm_name);
    if (strcmp(endpoint, "server") == 0) {
        std::cout << "Running serverside network test..." << std::endl;
        Key correct_key = Cascade::Key(size);
        if (size <= 10000)
            std::cout << "Server correct key: " << correct_key.to_string() << std::endl;
        else std::cout << "Server key ready" << std::endl;
        Server server(correct_key, true, host, port, user, pw, noise);
        printJsonServer(correct_key.to_string(),server.getTag());
        std::cout << "Error correction completed" << std::endl;

        return;
    } else if (strcmp(endpoint, "client") == 0) {
        std::cout << "Running clientside network test..." << std::endl;
        RabbitmqClassicalSession classical_session(host, port, user, pw);
        Client client(*algorithm, classical_session, size, noise);
        client.reconcile();
        Key &reconciled_key = client.get_reconciled_key();
        printJsonClient(noise, size, algorithm_name, client.get_stats());
        if (size < 100000)
            std::cout << "Client correct key: " << reconciled_key.to_string() << std::endl;
        return;
    } else {
        std::cout << "Argument 3 must be either 'sender' or 'receiver'" << std::endl;
    }

}


void runLocal(char *algorithm_name, int size, double noise) {
    std::cout << "Running local test..." << std::endl;
    const Algorithm *algorithm = Algorithm::get_by_name(algorithm_name);
    Key correct_key = Cascade::Key(size);
    MockClassicalSession classical_session(Server(correct_key, false));
    Client client(*algorithm, classical_session, size, noise);
    client.reconcile();
    Key &reconciled_key = client.get_reconciled_key();
    std::cout << "Differencies: " << correct_key.nr_bits_different(reconciled_key);
    return;
}

void printJsonClient(double errorRate, int nrBits, std::string algorithm, Stats stats) {
    DataPoint dataPoint(algorithm, nrBits, errorRate);
    dataPoint.record_reconciliation_stats(stats);
    std::string fileName = algorithm + "_" + std::to_string(nrBits) + "_" + std::to_string(errorRate) + ".json";
    auto myfile = std::fstream(fileName, std::ios::out);
    myfile.write(dataPoint.to_json().c_str(), dataPoint.to_json().size());
    myfile.close();
}

void printJsonServer(std::string key, std::string tag) {
    std::string fileName = "key.json";
    std:: string json="{ \"correctKey\": \"";
    json+= key+"\""+ ", ";
    json+="\" tag\": \"" + tag+"\"" ;
    json+="}";
    auto myfile = std::fstream(fileName, std::ios::out);
    myfile.write(json.c_str(), json.size());
    myfile.close();
}
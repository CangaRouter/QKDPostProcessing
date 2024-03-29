#include "algorithm.h"
#include "key.h"
#include "Client.h"
#include "Server.h"
#include "RabbitmqClassicalSession.h"
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>
#include <thread>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include "data_point.h"


using namespace Cascade;

void runOnRabbitmq(char *algorithm_name, int size, char *endpoint, double noise, const std::string& host, int port,
                   const std::string& user, const std::string& pw, std::string seq,DataPoint *dataPoint, std::string key);

void runLocal(char *algorithm_name, int size, double noise);

void printJsonClient(double errorRate, int nrBits, std::string algorithm, DataPoint *dataPoint,int runs=1);

void printJsonServer(std::string key, std::string tag);

int main(int argc, char **argv) {
    // [mode:network/local/many] [#bits] [algorithm] [endpoint:client/server] [noise] [host] [port] [user] [pw] [seq/runs] [seq/keyType] [key]
    // many: runs the protocol many times and prints the average, it also produces a json form with the averages value of the runs
    // network: runs the protocol on rabbitmq
    // local: runs the protocol locally (simulating a client-server architecture)
    // [#bits]: number of bits of the key
    // [algorithm]: name of the algorithm to use
    // [endpoint]: client or server
    // [noise]: noise to add to the key (QBER)
    // [host]: host of the rabbitmq server
    // [port]: port of the rabbitmq server
    // [user]: user of the rabbitmq server
    // [pw]: password of the rabbitmq server
    // [seq/runs]: this argument can either be the sequence of the protocol or the number of runs to do (depending on if you're running the algorithm one shot or for multiple testing)
    // [seq/keyType]: this argument can either be the sequence of the protocol or the  randomness type of the key (true if you want the server to generate a random key or false if you want to provide it in the next argument) (depending on if you're running the algorithm one shot or for multiple testing)
    // [key]: (only for server instances) this argument is the key to use for the protocol (if not provided a random key will be generated)
    if (argc <11 || argc >13) {
        std::perror(
                "Wrong number of arguments \n Usage ./Mycascade [mode:network/local/many] [#bits] [algorithm] [endpoint:client/server] [noise] [host] [port] [user] [pw] [seq/runs] [seq/ranndomKey] [key] ");
        for (int j = 0; j < argc; j++) {
            std::perror(argv[j]);
        }
        exit(-1);
    }
    std::string key;

    double noise = atof(argv[5]);
    int size = atoi(argv[2]);
    std::string host(argv[6]);
    int port = atoi(argv[7]);
    std::string user(argv[8]);
    std::string pw(argv[9]);
    char *algorithmName = argv[3];
    DataPoint *dataPoint=new DataPoint(algorithmName, size, noise);
    char *endpoint = argv[4];
    if (strcmp(argv[1], "network") == 0) {
        std::string seq(argv[10]);
        if(strcmp(endpoint,"server")==0){
            if(strcmp(argv[11],"false")==0 || strcmp(argv[11],"False")==0 || strcmp(argv[11],"FALSE")==0){
                key= std::string(argv[12]);
            }
        }
        runOnRabbitmq(algorithmName, size, endpoint, noise, host, port, user, pw, seq, dataPoint,key);
        if(strcmp(endpoint,"client")==0) {
            printJsonClient(noise, size, algorithmName, dataPoint);
        }
    } else if (strcmp(argv[1], "local") == 0) {
        runLocal(algorithmName, size, noise);
    } else if (strcmp(argv[1], "many") == 0) {
        int runs=std::stoi(argv[10]);
        std::string seq(argv[11]);
        for (int i=0;i<runs;i++){
            runOnRabbitmq(algorithmName, size, endpoint, noise, host, port, user, pw, seq, dataPoint,key);
            std::cout<<i<<std::endl;
        }
        if(strcmp(endpoint,"client")==0){
            dataPoint->computeAverages();
            printJsonClient(noise, size, algorithmName, dataPoint, runs);
        }
        return 0;

    } else
        std::cout << "argument 4 must be either network or local" << std::endl;
}


void runOnRabbitmq(char *algorithm_name, int size, char *endpoint, double noise, const std::string& host, int port,
                   const std::string& user, const std::string& pw,std::string seq,DataPoint *dataPoint, std::string key) {
    try {
        const Algorithm *algorithm = Algorithm::get_by_name(algorithm_name);
        if (strcmp(endpoint, "server") == 0) {
            Key correct_key = NULL;
            if(strlen(key.c_str())>0)
                correct_key = Key::parseKey(key);
            else
                correct_key = Cascade::Key(size);
            Server server(correct_key, true, host, port, user, pw, noise,seq);
            printJsonServer(correct_key.to_string(), server.getTag());
            return;
        } else if (strcmp(endpoint, "client") == 0) {
            RabbitmqClassicalSession classical_session(host, port, user, pw, seq);
            Client client(*algorithm, classical_session, size, noise);
            client.reconcile();
            dataPoint->record_reconciliation_stats(client.get_stats());
            return;
        } else {
            std::cout << "Argument 3 must be either 'sender' or 'receiver'" << std::endl;
        }
    } catch (...) {
        std::cerr << "QBER: " << noise << std::endl;
        std::cerr << "Key Size: " << size << std::endl;
        std::cerr << "Algorithm: " << algorithm_name << std::endl;
        std::exception_ptr p = std::current_exception();
        std::cerr << "Error: " << boost::current_exception_diagnostic_information() << std::endl;
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

void printJsonClient(double errorRate, int nrBits, std::string algorithm, DataPoint *dataPoint,int runs ) {
    std::string fileName = algorithm + "_" + std::to_string(nrBits*runs) + "_" + std::to_string(errorRate) + ".json";
    auto myfile = std::fstream(fileName, std::ios::out);
    myfile.write(dataPoint->to_json().c_str(), dataPoint->to_json().size());
    myfile.close();
}

void printJsonServer(std::string key, std::string tag) {
    std::string fileName = "key.json";
    std::string json = "{ \"correctKey\": \"";
    json += key + "\"" + ", ";
    json += "\" tag\": \"" + tag + "\"";
    json += "}";
    auto myfile = std::fstream(fileName, std::ios::out);
    myfile.write(json.c_str(), json.size());
    myfile.close();
}
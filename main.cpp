#include "algorithm.h"
//#include "data_point.h"
#include "debug.h"
//#include "experiments.h"
#include "key.h"
#include "classical_session.h"
//#include "options.h"
#include "random.h"
#include "report.h"
#include "Client.h"
#include "Server.h"
#include "RabbitmqClassicalSession.h"
//#include "series.h"
//#include <boost/filesystem.hpp>
#include <cerrno>
#include <deque>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <string.h>
#include <cstdlib>
#include <cassert>

using namespace Cascade;

int main(int argc, char **argv) {
    if (argc != 6) {
        std::perror("Wrong number of arguments");
    }
    if (strcmp(argv[1], "network") == 0) {
        std::string fileName = argv[5];

        char *algorithm_name = argv[3];
        int size = atoi(argv[2]);
        const Algorithm *algorithm = Algorithm::get_by_name(algorithm_name);


        if (strcmp(argv[4], "server") == 0) {
            Key correct_key = Cascade::Key(size);
            std::cout << correct_key.to_string() << std::endl;


            std::ofstream tmp_file("/tmp/" + fileName);
            if (!tmp_file.is_open()) {
                // Handle error opening the file
                return 1;
            }

            // Write the string to the temporary file
            tmp_file << correct_key.to_string();

            // Close the file
            tmp_file.close();
            Server server(correct_key, false, "localhost", 5672, "guest", "guest");
            return 0;
        } else if (strcmp(argv[4], "client") == 0) {
            std::string key;
            std::ifstream tmpFile("/tmp/" + fileName);
            if (tmpFile.is_open()) {
                tmpFile >> key;
                tmpFile.close();
            }
            Key correct_key = Cascade::Key::parseKey(key);
            std::cout <<"this is the parsed key: "<< key << std::endl;
            std::cout <<"and this is the parsed key: "<< correct_key.to_string() << std::endl;

            Key noisy_key = correct_key;
            double bit_error_rate = 0.6;
            noisy_key.apply_noise(bit_error_rate);
            RabbitmqClassicalSession classical_session("localhost", 5672, "guest", "guest");
            Client client(*algorithm, classical_session, noisy_key, bit_error_rate);
            client.reconcile();
            Key &reconciled_key = client.get_reconciled_key();
            std::cout << "Differencies: " << correct_key.nr_bits_different(reconciled_key);
            return 0;
        } else {
            std::cout << "Argument 3 must be either 'sender' or 'receiver'" << std::endl;
        }

    } else {
        std::string fileName = argv[5];

        char *algorithm_name = argv[3];
        int size = atoi(argv[2]);
        const Algorithm *algorithm = Algorithm::get_by_name(algorithm_name);


        Key correct_key = Cascade::Key(size);

        std::cout << correct_key.to_string() << std::endl;
        std::string keyy=correct_key.to_string();

        Key noisy_key = Key::parseKey(keyy);
        double bit_error_rate = 0.1;
        noisy_key.apply_noise(bit_error_rate);
        std::cout << noisy_key.to_string() << std::endl;
        MockClassicalSession classical_session(Server(correct_key, false));
        Client client(*algorithm, classical_session, noisy_key, bit_error_rate);
        client.reconcile();
        Key &reconciled_key = client.get_reconciled_key();
        std::cout << "Differencies: " << correct_key.nr_bits_different(reconciled_key);
        return 0;


    }


    /*int key_size= atoi(argv[1]);
    string algorithm_name=argv[2];
    int error_rate= atoi(argv[3]);
    //char* ip=argv[6];
    const Cascade::Algorithm* algorithm = Cascade::Algorithm::get_by_name(algorithm_name);
    assert(algorithm);
    Cascade::Key correct_key=Cascade::Key(key_size);
    Cascade::Key noisy_key=Cascade::Key(correct_key);
    noisy_key.apply_noise(error_rate);

    Cascade::MockClassicalSession classical_session(correct_key, algorithm->cache_shuffles);
    int actual_bit_errors = correct_key.nr_bits_different(noisy_key);
   // data_point.actual_bit_errors.record_value(actual_bit_errors);
    double actual_bit_error_rate = double(actual_bit_errors) / double(key_size);
  //  data_point.actual_bit_error_rate.record_value(actual_bit_error_rate);
    int remaining_bit_errors = 0;
    {
        // New scope to make sure reconciliation destructor is run before we reporting bit errors.
        Cascade::Client reconciliation(*algorithm, classical_session, noisy_key, error_rate);
        reconciliation.reconcile();
    //    data_point.record_reconciliation_stats(reconciliation.get_stats());
        remaining_bit_errors = correct_key.nr_bits_different(reconciliation.get_reconciled_key());
     //   data_point.remaining_bit_errors.record_value(remaining_bit_errors);
    }
    double remaining_bit_error_rate = double(remaining_bit_errors) / double(key_size);
    data_point.remaining_bit_error_rate.record_value(remaining_bit_error_rate);
     if (remaining_bit_errors > 0)
       data_point.remaining_frame_error_rate.record_value(1.0);
    else
        data_point.remaining_frame_error_rate.record_value(0.0);
    DEBUG("remaining_bit_errors=" << remaining_bit_errors);
    return 0;
    */
}

//
// Created by marco on 27/06/23.
//

#include "Client.h"

#include <utility>
static double elapsed_time(const struct timespec &start, const struct timespec &end) {
    double d_start = double(start.tv_sec) + double(start.tv_nsec) / 1000000000.0;
    double d_end = double(end.tv_sec) + double(end.tv_nsec) / 1000000000.0;
    return d_end - d_start;
}



Client::Client(ClassicalSession &classicalSession, const Key &key, std::string pw, std::string host,
               std::string user, std::string seq, int port, std::string mode, int size)
        : classical_session(classicalSession), key(key), pw(std::move(pw)), host(std::move(host)), user(std::move(user)), seq(std::move(seq)), port(port),
          mode(std::move(mode)), size(size) {}

double Client::PrivacyAmplification(int K, int M, int N, std::vector<uint64_t> initial_value){
    struct timespec start_process_time;
    int rc = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_process_time);
    assert(rc == 0);
    struct timespec start_real_time;
    rc = clock_gettime(CLOCK_MONOTONIC, &start_real_time);
    assert(rc == 0);

    if(mode == "cellular"){
        classical_session.NegotiateClient(K,M,N,initial_value);
        Key amplifiedKey= CellularAutomataPA::Amplificate(key,K,M,N,initial_value);
        classical_session.CloseClient();
        std::cout<<"amplified key: " <<amplifiedKey.to_string() <<std::endl;

    }
    struct timespec end_process_time;
    rc = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_process_time);
    assert(rc == 0);
    struct timespec end_real_time;
    rc = clock_gettime(CLOCK_MONOTONIC, &end_real_time);
    assert(rc == 0);
    double elapsed_process_time = elapsed_time(start_process_time, end_process_time);
    double elapsed_real_time = elapsed_time(start_real_time, end_real_time);



    return elapsed_real_time;
}

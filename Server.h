//
// Created by marco on 25/11/22.
//

#ifndef MYCASCADE_SERVER_H
#define MYCASCADE_SERVER_H

#include <map>
#include "key.h"
#include "shuffled_key.h"
#include "pending_item.h"
#include "iteration.h"
#include "classical_session.h"
#include <amqpcpp.h>
#include "conn_handler.h"

namespace Cascade {
    class Server {
    public:


        Server(Key &correctKey, bool cacheShuffles, ClassicalSession &channel);

        Server(Key &correctKey, bool cacheShuffles);

        Server(Key &correctKey, bool cacheShuffles, const std::string &host, int port, const std::string &user,
               const std::string &pw);

        int return_correct_parities(int iterationNr, int startBit, int endBit);

        void start_iteration_with_shuffle_seed(int iteration_nr, std::string shuffle_seed);

        virtual ~Server();

    private:
        void serverLoop();

        Key &correct_key;
        bool cache_shuffles;
        std::map<int, ShuffledKeyPtr> shuffled_keys;
        std::string host;
        int port;
        std::string user;
        std::string pw;


    };

} // Cascade

#endif //MYCASCADE_SERVER_H

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


namespace Cascade {

    class Server {
    public:


        Server(Key &correctKey, bool cacheShuffles);
        int return_correct_parities(int iterationNr, int startBit, int endBit);
        void start_iteration_with_shuffle_seed(int iteration_nr, std::string shuffle_seed);

        virtual ~Server();

    private:
        Key &correct_key;
        bool cache_shuffles;
        std::map<int, ShuffledKeyPtr> shuffled_keys;



    };

} // Cascade

#endif //MYCASCADE_SERVER_H

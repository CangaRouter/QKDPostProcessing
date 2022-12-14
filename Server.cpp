//
// Created by marco on 25/11/22.
//

#include "Server.h"

namespace Cascade {

    Server::Server(Key &correctKey, bool cacheShuffles) : correct_key(correctKey), cache_shuffles(cacheShuffles) {
        //TODO insert a classical session interface to allow server  to open the connection


    }

    Server::~Server() {

    }

    void Server::start_iteration_with_shuffle_seed(int iteration_nr,
                                                                 std::string shuffle_seed) {
        int nr_key_bits = correct_key.get_nr_bits();
        ShufflePtr shuffle;
        shuffle = Shuffle::new_shuffle_from_seed(iteration_nr, nr_key_bits, std::stoul(shuffle_seed),
                                                 cache_shuffles);
        ShuffledKeyPtr shuffled_key(new ShuffledKey(correct_key, shuffle));
        shuffled_keys[iteration_nr] = shuffled_key;
    }


    int Server::return_correct_parities(int iterationNr, int startBit, int endBit) {
        // Once we implement the real classical session, we will need to keep track of the blocks
        // for which we asked Alice the correct parity, but for which we have not yet received the
        // answer from Alice. For now, assume we get the answer immediately.
            ShuffledKeyPtr shuffled_key = shuffled_keys[iterationNr];
            int correct_parity = shuffled_key->compute_range_parity(startBit,
                                                                    endBit);
            return correct_parity;
    }



} // Cascade
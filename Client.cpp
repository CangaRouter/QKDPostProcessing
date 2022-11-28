//
// Created by marco on 25/11/22.
//

#include "Client.h"

namespace Cascade {



    void Client::start_iteration_with_shuffle_seed(int iteration_nr,
                                                             uint64_t shuffle_seed) {
        int nr_key_bits = correct_key.get_nr_bits();
        ShufflePtr shuffle;
        shuffle = Shuffle::new_shuffle_from_seed(iteration_nr, nr_key_bits, shuffle_seed,
                                                 cache_shuffles);
        ShuffledKeyPtr shuffled_key(new ShuffledKey(correct_key, shuffle));
        shuffled_keys[iteration_nr] = shuffled_key;
    }
} // Cascade
//
// Created by marco on 25/11/22.
//

#include "Server.h"

namespace Cascade {

    Server::Server(Key &correctKey, bool cacheShuffles) : correct_key(correctKey), cache_shuffles(cacheShuffles) {


    }

    Server::~Server() {

    }

    void Server::ask_correct_parities(PendingItemQueue &ask_correct_parity_blocks) {
        // Once we implement the real classical session, we will need to keep track of the blocks
        // for which we asked Alice the correct parity, but for which we have not yet received the
        // answer from Alice. For now, assume we get the answer immediately.
        for (auto it = ask_correct_parity_blocks.begin(); it != ask_correct_parity_blocks.end(); ++it) {
            PendingItem pending_item(*it);
            BlockPtr block = pending_item.block;
            int iteration_nr = block->get_iteration().get_iteration_nr();
            ShuffledKeyPtr shuffled_key = shuffled_keys[iteration_nr];
            int correct_parity = shuffled_key->compute_range_parity(block->get_start_bit_nr(),
                                                                    block->get_end_bit_nr());
            block->set_correct_parity(correct_parity);
        }
    }


} // Cascade
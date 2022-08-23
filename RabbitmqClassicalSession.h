//
// Created by Marco Pappalardo on 21/07/2022.
//

#ifndef MYCASCADE_RABBITMQCLASSICALSESSION_H
#define MYCASCADE_RABBITMQCLASSICALSESSION_H
#include "classical_session.h"
#include "shuffled_key.h"
#include <map>
#include "block.h"
#include "pending_item.h"
#include "shuffle.h"

#include <cstdint>
namespace Cascade {

    class RabbitmqClassicalSession : public ClassicalSession{
    public:
        RabbitmqClassicalSession(bool receiver,Key &key, bool cache_shuffles);

        virtual ~RabbitmqClassicalSession() ;

        virtual void start_iteration_with_shuffle_seed(int iteration_nr, uint64_t shuffle_seed) ;

        virtual void start_iteration_with_shuffle(int iteration_nr, ShufflePtr shuffle) ;

        virtual void ask_correct_parities(PendingItemQueue &ask_correct_parity_blocks) ;

    };
}

#endif //MYCASCADE_RABBITMQCLASSICALSESSION_H

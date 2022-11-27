//
// Created by marco on 25/11/22.
//

#ifndef MYCASCADE_SENDER_H
#define MYCASCADE_SENDER_H

#include <map>
#include "key.h"
#include "shuffled_key.h"
#include "classical_session.h"

namespace Cascade {

    class Sender {
    public:

        Sender(Key &correctKey, bool cacheShuffles, const ClassicalSession &channel);

        virtual ~Sender();

    private:
        Key &correct_key;
        bool cache_shuffles;
        std::map<int, ShuffledKeyPtr> shuffled_keys;
        ClassicalSession channel;

        int seq=0;
        int senderbit= -1;
        int receiverbit=-1;
    };

} // Cascade

#endif //MYCASCADE_SENDER_H

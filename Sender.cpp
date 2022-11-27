//
// Created by marco on 25/11/22.
//

#include "Sender.h"

namespace Cascade {

    Sender::Sender(Key &correctKey, bool cacheShuffles, const ClassicalSession &channel) : correct_key(correctKey),
                                                                                           cache_shuffles(
                                                                                                   cacheShuffles),
                                                                                           channel(channel) {}

    Sender::~Sender() {

    }


    x
    ClassicalSession classical_session(correct_key, algorithm->cache_shuffles, "localhost","5672","e","q2");
    classical_session.openSenderChannel();


} // Cascade
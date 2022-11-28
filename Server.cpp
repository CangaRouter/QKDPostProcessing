//
// Created by marco on 25/11/22.
//

#include "Server.h"

namespace Cascade {

    Server::Server(Key &correctKey, bool cacheShuffles, const ClassicalSession &channel) : correct_key(correctKey),
                                                                                           cache_shuffles(
                                                                                                   cacheShuffles),
                                                                                           channel(channel) {}

    Server::~Server() {

    }


    x
    ClassicalSession classical_session(correct_key, algorithm->cache_shuffles, "localhost","5672","e","q2");
    classical_session.openSenderChannel();


} // Cascade
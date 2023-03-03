//
// Created by marco on 25/11/22.
//

#include <sstream>
#include "Server.h"


namespace Cascade {


    Server::Server(Key &correctKey, bool cacheShuffles) : correct_key(correctKey), cache_shuffles(cacheShuffles) {


    }


    Server::Server(Key &correctKey, bool cacheShuffles, const std::string &host, int port, const std::string &user,
                   const std::string &pw, double noise) : correct_key(correctKey), cache_shuffles(cacheShuffles),
                                                          host(host),
                                                          port(port), user(user), pw(pw), noise(noise) {

        serverLoop();

    }


    Server::~Server() = default;

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

    void Server::serverLoop() {
        ConnHandler handler;
        AMQP::TcpConnection connection(handler,
                                       AMQP::Address(host, port,
                                                     AMQP::Login(user, pw), "/"));
        AMQP::TcpChannel channel(&connection);
        channel.onError([&handler](const char *message) {
            std::cout << "Channel error: " << message << std::endl;
            handler.Stop();
        });
        channel.setQos(1);
        channel.declareQueue("serverQueue");
        channel.consume("serverQueue", "alfredo", AMQP::noack + AMQP::exclusive)
                .onReceived
                        (
                                [&, this](const AMQP::Message &m, uint64_t tag, bool) {
                                    int startBit = -1;
                                    int endBit = -1;
                                    int nIteration = -1;
                                    AMQP::Table headers = m.headers();
                                    std::string messageType = headers.get("messageType");

                                    if (strcmp(messageType.c_str(), "startIteration") == 0) {
                                        std::string seed = headers.get("seed");
                                        nIteration = headers.get("nIteration");
                                        start_iteration_with_shuffle_seed(nIteration, seed);
                                    } else if (strcmp(messageType.c_str(), "cascade_rpc") == 0) {
                                        startBit = headers.get("startBit");
                                        endBit = headers.get("endBit");
                                        nIteration = headers.get("nIteration");
                                        int response = return_correct_parities(nIteration, startBit, endBit);
                                        AMQP::Envelope env("", 0);
                                        AMQP::Table headers;
                                        headers.set("parity", response);
                                        env.setHeaders(headers);
                                        env.setCorrelationID(m.correlationID());
                                        channel.publish("", m.replyTo(), env);
                                    } else if (strcmp(messageType.c_str(), "initialization") == 0) {
                                        Key noisyKey(correct_key);
                                        noisyKey.apply_noise(noise);
                                        AMQP::Envelope env("", 0);
                                        AMQP::Table headers;
                                        headers.set("messageType", "initializationResponse");
                                        headers.set("key", noisyKey.to_string());
                                        env.setHeaders(headers);
                                        env.setCorrelationID(m.correlationID());
                                        channel.publish("", m.replyTo(), env);
                                    } else if (strcmp(messageType.c_str(), "closing") == 0) {
                                        AMQP::Envelope env("", 0);
                                        AMQP::Table headers;
                                        headers.set("messageType", "closingConfirm");
                                        env.setHeaders(headers);
                                        env.setCorrelationID(m.correlationID());
                                        channel.publish("", m.replyTo(), env);
                                        channel.cancel("alfredo");
                                        channel.removeQueue("serverQueue");
                                        handler.Stop();

                                    } else {
                                        std::cout << "unrecognized message type: " << tag << std::endl;
                                    }

                                }

                        ).onCancelled([&](const std::string &consumertag) {
                    handler.Stop();

                });
        handler.Start();
        connection.close();
    }


    const Key &Server::getCorrectKey() const {
        return correct_key;
    }

    const std::string &Server::getHost() const {
        return host;
    }

    int Server::getPort() const {
        return port;
    }

    const std::string &Server::getUser() const {
        return user;
    }

    const std::string &Server::getPw() const {
        return pw;
    }

    double Server::getNoise() const {
        return noise;
    }


} // Cascade